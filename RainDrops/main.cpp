#include "Bethesda/BSMemory.hpp"
#include "Bethesda/BSShader.hpp"

#include "Bethesda/BSShaderManager.hpp"
#include "Bethesda/ImageSpaceManager.hpp"
#include "Bethesda/ImageSpaceEffect.hpp"
#include "Bethesda/ImageSpaceShader.hpp"
#include "Bethesda/ImageSpaceShaderParam.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/ShadowSceneNode.hpp"
#include "Bethesda/Sky.hpp"
#include "Bethesda/TESMain.hpp"
#include "Bethesda/TimeGlobal.hpp"
#include "Gamebryo/NiDX9Renderer.hpp"
#include "RainRaycast/RainRaycast.hpp"
#include "Misc/CustomGameSetting.hpp"

#include "nvse/PluginAPI.h"
#include "simpleini/SimpleIni.h"

BS_ALLOCATORS

IDebugLog gLog("logs\\RainDrops.log");

// Hook related globals.
static CallDetour				kClearRainDetour;

// Constants.
static NVSEMessagingInterface*	pMsgInterface = nullptr;
static uint32_t					uiPluginHandle = 0;
static constexpr uint32_t		uiShaderLoaderVersion = 140;

const char sRainBlur[] = "RAINBLUR";
const char sRainDropsBlur[] = "RAINDROPSBLUR";
const char sRainDrops[] = "RAINDROPS";

// Statics.
typedef void(__cdecl pfn_RegisterEOFEffect)(uint32_t auiIndex, ImageSpaceEffect* apEffect);
static pfn_RegisterEOFEffect* pRegisterEOFEffect = nullptr;

namespace RTM {

	enum ImageSpaceStage : uint32_t {
		IS_NONE = 0, // No IS effects are currently being rendered
		IS_BG = 1,   // IS effects for the background are being rendered
		IS_FG = 2,   // IS effects for the foreground are being rendered
	};

	typedef ImageSpaceStage(__cdecl pfn_GetImageSpaceStage)();

	static pfn_GetImageSpaceStage* pGetImageSpaceStage;

	static ImageSpaceStage DefaultGetImageSpaceStage() {
		return IS_NONE;
	}
}

struct RainDropSettings {
	CustomGameSetting   bEnabled;
	CustomGameSetting   b1stPersonOnly;
	CustomGameSetting	fDensity;
	CustomGameSetting   fSize;
	CustomGameSetting	bEnableStaticDrops;
	CustomGameSetting	bEnableMovingDrops;
	CustomGameSetting	bEnableFogging;
	CustomGameSetting	bEnableFogBlurring;
	CustomGameSetting	bEnableFogColor;
	CustomGameSetting	bEnableFogTrails;
	CustomGameSetting	fFogColorInfluence;
	CustomGameSetting	fFadeIn;
	CustomGameSetting	fFadeOut;
};

struct SettingOverrides {
	CustomGameSetting   bEnabledOverride;
	CustomGameSetting	bEnableFoggingOverride;
	CustomGameSetting	fDensityMultiplier;
};

static RainDropSettings	kRainDropSettings[2];
static SettingOverrides	kRainDropSettingOverrides;

static BSRenderedTexturePtr spBlurBufferRT = nullptr;

static ImageSpaceShader* pShaderRainBlur = nullptr;
static ImageSpaceShader* pShaderRainDropsBlur = nullptr;
static ImageSpaceShader* pShaderRainDrops = nullptr;

template<const char* sName, uint32_t uiTextures>
class ImageSpaceShaderTemplate : public ImageSpaceShader {
private:
	static void* BSShader_vtable[85];
	static void* ImageSpaceEffect_vtable[8];
public:
	static ImageSpaceShaderTemplate<sName, uiTextures>* CreateObject() {
		ImageSpaceShaderTemplate<sName, uiTextures>* pShader = NiNew<ImageSpaceShaderTemplate<sName, uiTextures>>();

		// Call constructor for ImageSpaceShader and set vtable pointers.
#if GAME
		ThisCall(0xC04570, pShader);
#else
		ThisCall(0x977EE0, pShader);
#endif

		if (!BSShader_vtable[0]) [[unlikely]] {
			for (uint32_t i = 0; i < _countof(BSShader_vtable); i++) {
				BSShader_vtable[i] = ((void***)pShader)[0][i];
			}

			ReplaceVTableEntry(BSShader_vtable, 72, &ImageSpaceShaderTemplate<sName, uiTextures>::ReloadShadersEx);
		}
		((DWORD*)pShader)[0] = (DWORD)BSShader_vtable;

		if (!ImageSpaceEffect_vtable[0]) [[unlikely]] {
			for (uint32_t i = 0; i < _countof(ImageSpaceEffect_vtable); i++) {
				ImageSpaceEffect_vtable[i] = ((void***)pShader + 27)[0][i];
			}
		}
		((DWORD*)pShader + 27)[0] = (DWORD)ImageSpaceEffect_vtable;

		pShader->kTextures.SetSize(uiTextures);

		return pShader;
	}

	bool ReloadShadersEx() {
		LoadShadersEx(sName);
		return PresetStages();
	}
};

template<const char* sName, uint32_t uiTextures>
void* ImageSpaceShaderTemplate<sName, uiTextures>::BSShader_vtable[85] = {};

template<const char* sName, uint32_t uiTextures>
void* ImageSpaceShaderTemplate<sName, uiTextures>::ImageSpaceEffect_vtable[8] = {};

template class ImageSpaceShaderTemplate<sRainBlur, 2>;
template class ImageSpaceShaderTemplate<sRainDropsBlur, 3>;
template class ImageSpaceShaderTemplate<sRainDrops, 2>;

using ImageSpaceShaderRainBlur = ImageSpaceShaderTemplate<sRainBlur, 2>;
using ImageSpaceShaderRainDropsBlur = ImageSpaceShaderTemplate<sRainDropsBlur, 3>;
using ImageSpaceShaderRainDrops = ImageSpaceShaderTemplate<sRainDrops, 2>;

// Math funcs.
float Linstep(float afEdge0, float afEdge1, float afX) {
	return std::clamp((afX - afEdge0) / (afEdge1 - afEdge0), 0.0f, 1.0f);
}

float Smoothstep(float afEdge0, float afEdge1, float afX) {
	afX = std::clamp((afX - afEdge0) / (afEdge1 - afEdge0), 0.0f, 1.0f);
	return afX * afX * (3 - 2 * afX);
}

class ImageSpaceEffectRainDrops : public ImageSpaceEffect {
private:
	static void* vtable[8];

	bool	bStatus;
	bool	bStopping;
	float	fTimer;
	float	fTimerStop;
	float	fRainAmount;
	float	fRainAmountStop;
public:
	ImageSpaceTexture kBuffer;

	static ImageSpaceEffectRainDrops* CreateObject() {
		ImageSpaceEffectRainDrops* pEffect = NiNew<ImageSpaceEffectRainDrops>();

		// Call constructor for ImageSpaceEffect and set vtable pointers.
#if GAME
		ThisCall(0xBA4080, pEffect);
#else
		ThisCall(0x938840, pEffect);
#endif

		if (!vtable[0]) [[unlikely]] {
			for (uint32_t i = 0; i < _countof(vtable); i++) {
				vtable[i] = ((void***)pEffect)[0][i];
			}

			ReplaceVTableEntry(vtable, 1, &ImageSpaceEffectRainDrops::RenderEx);
			ReplaceVTableEntry(vtable, 2, &ImageSpaceEffectRainDrops::SetupEx);
			ReplaceVTableEntry(vtable, 4, &ImageSpaceEffectRainDrops::BorrowTexturesEx);
			ReplaceVTableEntry(vtable, 5, &ImageSpaceEffectRainDrops::ReturnTexturesEx);
			ReplaceVTableEntry(vtable, 6, &ImageSpaceEffectRainDrops::IsActiveEx);
			ReplaceVTableEntry(vtable, 7, &ImageSpaceEffectRainDrops::UpdateParamsEx);

		}
		((DWORD*)pEffect)[0] = (DWORD)vtable;

		pEffect->kTextures.SetSize(3);

		return pEffect;
	}

	void RenderEx(NiTriShape* apScreenShape, NiDX9Renderer* apRenderer, ImageSpaceEffectParam* apParam, bool abEndFrame) {
		BorrowTextures(apParam);

		Sky* pSky = Sky::GetSingleton();

		if (!pSky)
			return;

		RainDropSettings kSettings = kRainDropSettings[pSky->GetIsSnowing() ? 1 : 0];

		int iStart = 0;
		int iEnd = 3;

		if (!kSettings.bEnableFogBlurring.Bool() || !kRainDropSettingOverrides.bEnableFoggingOverride.Bool()) {
			iStart = 3;
			iEnd = 4;
		}

		for (int i = iStart; i < iEnd; i++) {
			ApplyEffectTextures(i);
			UpdateParams(apParam);
			kEffects.GetAt(i)->Render(apScreenShape, apRenderer, kEffectParams.GetAt(i), abEndFrame);
		}

		ReturnTextures();
	}

	void SetupEx(ImageSpaceManager* pISManager, ImageSpaceEffectParam* apParam) {
		SetEffectCount(4);

		NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();

		uint32_t uiScreenWidth, uiScreenHeight;

		if (BSShaderManager::bLetterBox) {
			uiScreenWidth = BSShaderManager::iLetterboxWidth;
			uiScreenHeight = BSShaderManager::iLetterboxHeight;
		}
		else {
			uiScreenWidth = pRenderer->GetScreenWidth();
			uiScreenHeight = pRenderer->GetScreenHeight();
		}

		float fTexelWidth = 1.0f / uiScreenWidth;
		float fTexelHeight = 1.0f / uiScreenHeight;

		// Blur the rendered screen.
		ImageSpaceShaderParam* pBlurVerticalParam = ImageSpaceShaderParam::CreateObject();
		pBlurVerticalParam->ResizeConstantGroup(0, 2);
		pBlurVerticalParam->SetPixelConstants(0, fTexelWidth, fTexelHeight, 0.0f, 1.0f);
		SetEffect(0, pShaderRainBlur, pBlurVerticalParam, 2);
		SetEffectInputs(0, 1, NiTexturingProperty::FILTER_NEAREST);

		ImageSpaceShaderParam* pBlurHorizontalParam = ImageSpaceShaderParam::CreateObject();
		pBlurHorizontalParam->ResizeConstantGroup(0, 2);
		pBlurHorizontalParam->SetPixelConstants(0, fTexelWidth, fTexelHeight, 1.0f, 0.0f);
		SetEffect(1, pShaderRainBlur, pBlurHorizontalParam, 2);
		SetEffectInputs(1, 2, NiTexturingProperty::FILTER_NEAREST);

		// Generate rain drops with blur.
		ImageSpaceShaderParam* pRainDropsParam = ImageSpaceShaderParam::CreateObject();
		pRainDropsParam->ResizeConstantGroup(0, 5);
		pRainDropsParam->SetPixelConstants(0, fTexelWidth, fTexelHeight, uiScreenWidth, uiScreenHeight);
		SetEffect(2, pShaderRainDropsBlur, pRainDropsParam, 0);
		SetEffectInputs(2, 1, 2, NiTexturingProperty::FILTER_BILERP, NiTexturingProperty::FILTER_BILERP);

		// Generate rain drops without blur.
		SetEffect(3, pShaderRainDrops, pRainDropsParam, 0);
		SetEffectInputs(3, 1, NiTexturingProperty::FILTER_BILERP);
	}

	void BorrowTexturesEx(ImageSpaceEffectParam* apParam) {
		kBuffer.SetTexture(spBlurBufferRT);

		kTextures.SetAt(2, &kBuffer);
	}

	void ReturnTexturesEx() {
		kTextures.GetAt(2)->ClearTexture();

		kTextures.SetAt(2, nullptr);
	}

	bool IsActiveEx() {
		if (TESMain::GetSingleton()->bRenderingMenuBG || RTM::pGetImageSpaceStage() == RTM::IS_BG)
			return false;

		Sky* pSky = Sky::GetSingleton();

		if (!pSky || pSky->IsUnderwater()) {
			Stop();
			return false;
		}

		bool bIsRaining = pSky->GetIsRaining();
		bool bIsSnowing = pSky->GetIsSnowing();
		bool bIsWet = bIsRaining || bIsSnowing;

		RainDropSettings kSettings = kRainDropSettings[bIsSnowing ? 1 : 0];
		
		bool bEnabled = kSettings.bEnabled.Bool() && kRainDropSettingOverrides.bEnabledOverride.Bool();

		if (bIsWet && !bEnabled) {
			Stop();
			return false;
		}

		if (kSettings.b1stPersonOnly.Bool() && PlayerCharacter::GetSingleton()->Is3rdPerson()) {
			Stop();
			return false;
		}

		bool bNewStatus = (bIsRaining || bIsSnowing) && !BSShaderManager::bIsInInterior && !RainRaycast::bHit;

		if (bStatus) {
			if (!bNewStatus && !bStopping) {
				bStopping = true;
				fTimerStop = fTimer;
				fRainAmountStop = fRainAmount;
			}
			else if (bNewStatus) {
				bStopping = false;
			}

			return true;
		}
		else if (bNewStatus) {
			bStatus = true;
			bStopping = false;
			return true;
		}

		Stop();
		return false;
	}

	bool UpdateParamsEx(ImageSpaceEffectParam* apParam) {
		Sky* pSky = Sky::GetSingleton();

		if (!pSky)
			return false;

		RainDropSettings kSettings = kRainDropSettings[pSky->GetIsSnowing() ? 1 : 0];

		TimeGlobal* pTimeGlobal = TimeGlobal::GetSingleton();

		fTimer += pTimeGlobal->fDelta;

		if (!bStopping)
			fRainAmount = Linstep(0.0f, kSettings.fFadeIn.Float(), fTimer);
		else
			fRainAmount = std::max(0.0f, fRainAmountStop - Linstep(fTimerStop, fTimerStop + kSettings.fFadeOut.Float(), fTimer));

		float fStaticScale = Smoothstep(-0.5f, 1.0f, fRainAmount) * 2.0f;
		float fMoving1Scale = Smoothstep(0.25f, 0.75f, fRainAmount);
		float fMoving2Scale = Smoothstep(0.0f, 0.5f, fRainAmount);

		NiColor kFogColor = NiColor(1, 1, 1);
		float fFogStrength = 0.0f;

		BSFogProperty* pFogProp = ShadowSceneNode::GetFogProperty(BSShaderManager::GetFogIndex());
		if (pFogProp) {
			kFogColor = pFogProp->m_kColor;
			fFogStrength = fRainAmount;
		}

		float fDensity = kSettings.fDensity.Float() * kRainDropSettingOverrides.fDensityMultiplier.Float();

		ImageSpaceShaderParam* pRainDropsParam = static_cast<ImageSpaceShaderParam*>(kEffectParams.GetAt(2));
		pRainDropsParam->SetPixelConstants(1, fTimer, fStaticScale, fMoving1Scale, fMoving2Scale);
		pRainDropsParam->SetPixelConstants(2, fDensity, kSettings.fSize.Float(), kSettings.bEnableStaticDrops.Bool(), kSettings.bEnableMovingDrops.Bool());
		pRainDropsParam->SetPixelConstants(3, kSettings.bEnableFogging.Bool(), kSettings.bEnableFogTrails.Bool(), kSettings.fFogColorInfluence.Float(), 0.0f);
		pRainDropsParam->SetPixelConstants(4, kFogColor.r, kFogColor.g, kFogColor.b, fFogStrength);

		if (bStopping && fRainAmount < 0.01f)
			Stop();

		return true;
	}

	void Stop() {
		bStatus = false;
		bStopping = false;
		fTimer = 0.0f;
		fTimerStop = 0.0f;
		fRainAmount = 0.0f;
		fRainAmountStop = 0.0f;
	}

	void Reset() {
		fTimer = 0.0f;
		fTimerStop = 0.0f;
		fRainAmount = 0.0f;
		fRainAmountStop = 0.0f;
	}
};

void* ImageSpaceEffectRainDrops::vtable[8] = {};

static ImageSpaceEffectRainDrops* pEffectRainDrops = nullptr;

void LoadSettings() {
	CSimpleIniA kIni;
	kIni.SetUnicode();
	SI_Error rc = kIni.LoadFile("Data\\NVSE\\Plugins\\RainDrops.ini");
	if (rc < 0) {
		_MESSAGE("No INI, using default values.");
	};

	kRainDropSettings[0].bEnabled.Initialize("bISRainDropsEnabled", static_cast<bool>(kIni.GetBoolValue("Rain", "bEnabled", true)));
	kRainDropSettings[0].b1stPersonOnly.Initialize("bISRainDrops1stPersonOnly", static_cast<bool>(kIni.GetBoolValue("Rain", "b1stPersonOnly", true)));
	kRainDropSettings[0].fDensity.Initialize("fISRainDropsDensity", static_cast<float>(kIni.GetDoubleValue("Rain", "fDensity", 0.5f)));
	kRainDropSettings[0].fSize.Initialize("fISRainDropsSize", static_cast<float>(kIni.GetDoubleValue("Rain", "fSize", 1.0f)));
	kRainDropSettings[0].bEnableStaticDrops.Initialize("bISRainDropsEnableStaticDrops", static_cast<bool>(kIni.GetBoolValue("Rain", "bEnableStaticDrops", true)));
	kRainDropSettings[0].bEnableMovingDrops.Initialize("bISRainDropsEnableMovingDrops", static_cast<bool>(kIni.GetBoolValue("Rain", "bEnableMovingDrops", true)));
	kRainDropSettings[0].bEnableFogging.Initialize("bISRainDropsEnableFogging", static_cast<bool>(kIni.GetBoolValue("Rain", "bEnableFogging", true)));
	kRainDropSettings[0].bEnableFogBlurring.Initialize("bISRainDropsEnableFogBlurring", static_cast<bool>(kIni.GetBoolValue("Rain", "bEnableFogBlurring", true)));
	kRainDropSettings[0].bEnableFogTrails.Initialize("bISRainDropsEnableFogTrails", static_cast<bool>(kIni.GetBoolValue("Rain", "bEnableFogTrails", true)));
	kRainDropSettings[0].fFogColorInfluence.Initialize("fISRainDropsFogColorInfluence", static_cast<float>(kIni.GetDoubleValue("Rain", "fFogColorInfluence", 0.1f)));
	kRainDropSettings[0].fFadeIn.Initialize("fISRainDropsFadeIn", static_cast<float>(kIni.GetDoubleValue("Rain", "fFadeIn", 30.0f)));
	kRainDropSettings[0].fFadeOut.Initialize("fISRainDropsFadeOut", static_cast<float>(kIni.GetDoubleValue("Rain", "fFadeOut", 5.0f)));

	kRainDropSettings[1].bEnabled.Initialize("bISSnowDropsEnabled", static_cast<bool>(kIni.GetBoolValue("Snow", "bEnabled", true)));
	kRainDropSettings[1].b1stPersonOnly.Initialize("bISSnowDrops1stPersonOnly", static_cast<bool>(kIni.GetBoolValue("Snow", "b1stPersonOnly", true)));
	kRainDropSettings[1].fDensity.Initialize("fISSnowDropsDensity", static_cast<float>(kIni.GetDoubleValue("Snow", "fDensity", 0.25f)));
	kRainDropSettings[1].fSize.Initialize("fISSnowDropsSize", static_cast<float>(kIni.GetDoubleValue("Snow", "fSize", 0.5f)));
	kRainDropSettings[1].bEnableStaticDrops.Initialize("bISSnowDropsEnableStaticDrops", static_cast<bool>(kIni.GetBoolValue("Snow", "bEnableStaticDrops", true)));
	kRainDropSettings[1].bEnableMovingDrops.Initialize("bISSnowDropsEnableMovingDrops", static_cast<bool>(kIni.GetBoolValue("Snow", "bEnableMovingDrops", false)));
	kRainDropSettings[1].bEnableFogging.Initialize("bISSnowDropsEnableFogging", static_cast<bool>(kIni.GetBoolValue("Snow", "bEnableFogging", true)));
	kRainDropSettings[1].bEnableFogBlurring.Initialize("bISSnowDropsEnableFogBlurring", static_cast<bool>(kIni.GetBoolValue("Snow", "bEnableFogBlurring", true)));
	kRainDropSettings[1].bEnableFogTrails.Initialize("bISSnowDropsEnableFogTrails", static_cast<bool>(kIni.GetBoolValue("Snow", "bEnableFogTrails", true)));
	kRainDropSettings[1].fFogColorInfluence.Initialize("fISSnowDropsFogColorInfluence", static_cast<float>(kIni.GetDoubleValue("Snow", "fFogColorInfluence", 0.1f)));
	kRainDropSettings[1].fFadeIn.Initialize("fISSnowDropsFadeIn", static_cast<float>(kIni.GetDoubleValue("Snow", "fFadeIn", 30.0f)));
	kRainDropSettings[1].fFadeOut.Initialize("fISSnowDropsFadeOut", static_cast<float>(kIni.GetDoubleValue("Snow", "fFadeOut", 5.0f)));

	kRainDropSettingOverrides.bEnabledOverride.Initialize("bISDropsEnabledOverride", true);
	kRainDropSettingOverrides.bEnableFoggingOverride.Initialize("bISDropsEnableFoggingOverride", true);
	kRainDropSettingOverrides.fDensityMultiplier.Initialize("fISDropsDensityMultiplier", 1.0f);
}

void InitializeTextures() {
	NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();

	uint32_t uiScreenWidth, uiScreenHeight;

	if (BSShaderManager::bLetterBox) {
		uiScreenWidth = BSShaderManager::iLetterboxWidth;
		uiScreenHeight = BSShaderManager::iLetterboxHeight;
	}
	else {
		uiScreenWidth = pRenderer->GetScreenWidth();
		uiScreenHeight = pRenderer->GetScreenHeight();
	}

	constexpr uint32_t eCreationFlags = BSTextureManager::BSTM_CF_NO_DEPTH | BSTextureManager::BSTM_CF_NO_STENCIL;
	constexpr D3DFORMAT eFormat = D3DFMT_X8R8G8B8;
	spBlurBufferRT = BSShaderManager::GetTextureManager()->CreateRenderedTexture(NiDX9Renderer::GetSingleton(), uiScreenWidth / 2, uiScreenHeight / 2, eCreationFlags, eFormat, D3DFMT_UNKNOWN, BSTextureManager::BSTM_RT_CUSTOM, D3DMULTISAMPLE_NONE, nullptr, 0);
	if (spBlurBufferRT)
		spBlurBufferRT->GetTexture(0)->SetName("RainBlur");
}

void InitializeShaders() {
	pShaderRainBlur = ImageSpaceShaderRainBlur::CreateObject();
	pShaderRainDropsBlur = ImageSpaceShaderRainDropsBlur::CreateObject();
	pShaderRainDrops = ImageSpaceShaderRainDrops::CreateObject();
	pEffectRainDrops = ImageSpaceEffectRainDrops::CreateObject();

	ImageSpaceManager* pISManager = ImageSpaceManager::GetSingleton();

	pShaderRainBlur->ReloadShaders();
	pShaderRainDropsBlur->ReloadShaders();
	pShaderRainDrops->ReloadShaders();
	pEffectRainDrops->Setup(pISManager, nullptr);

	pRegisterEOFEffect(12099, pEffectRainDrops);
}

void __cdecl ClearRain() {
	CdeclCall(kClearRainDetour.GetOverwrittenAddr());

	pEffectRainDrops->Reset();
}

void InitHooks() {
	kClearRainDetour.ReplaceCall(0x5D8923, ClearRain);
}

void MessageHandler(NVSEMessagingInterface::Message* msg) {
	switch (msg->type) {
	case NVSEMessagingInterface::kMessage_DeferredInit:
		InitializeTextures();
		InitializeShaders();

		RainRaycast::Initialize();

		break;
	case NVSEMessagingInterface::kMessage_PostLoadGame:
		pEffectRainDrops->Stop();

		break;
	case NVSEMessagingInterface::kMessage_MainGameLoop:
		RainRaycast::StartThread();

		break;
	}
}

void ShaderLoaderMessageHandler(NVSEMessagingInterface::Message* apMessage) {
	switch (apMessage->type) {
	case 0:
		pShaderRainBlur->ReloadShaders();
		pShaderRainDropsBlur ->ReloadShaders();
		pShaderRainDrops->ReloadShaders();
		break;
	}
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info) {
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "Rain Drops";
	info->version = 100;

	return !nvse->isEditor;
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Load(NVSEInterface* nvse) {
	HMODULE hShaderLoader = GetModuleHandle("Fallout Shader Loader.dll");

	if (!hShaderLoader) {
		MessageBox(NULL, "Fallout Shader Loader not found.\nRain Drops cannot be used without it, please install it.", "Rain Drops", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	pMsgInterface = (NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging);
	uiPluginHandle = nvse->GetPluginHandle();
	pMsgInterface->RegisterListener(uiPluginHandle, "NVSE", MessageHandler);
	pMsgInterface->RegisterListener(uiPluginHandle, "Shader Loader", ShaderLoaderMessageHandler);

	auto pQuery = (_NVSEPlugin_Query)GetProcAddress(hShaderLoader, "NVSEPlugin_Query");
	PluginInfo kInfo = {};
	pQuery(nvse, &kInfo);
	if (kInfo.version < uiShaderLoaderVersion) {
		char cBuffer[192];
		sprintf_s(cBuffer, "Fallout Shader Loader is outdated.\nPlease update it to use Rain Drops!\nCurrent version: %i\nMinimum required version: %i", kInfo.version, uiShaderLoaderVersion);
		MessageBox(NULL, cBuffer, "Rain Drops", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	BSShader::pCreateVertexShader = (pfn_CreateVertexShader*)GetProcAddress(hShaderLoader, "CreateVertexShader");
	BSShader::pCreatePixelShader = (pfn_CreatePixelShader*)GetProcAddress(hShaderLoader, "CreatePixelShader");
	pRegisterEOFEffect = (pfn_RegisterEOFEffect*)GetProcAddress(hShaderLoader, "RegisterEOFEffect");

	if (!BSShader::pCreatePixelShader || !BSShader::pCreateVertexShader || !pRegisterEOFEffect) {
		MessageBox(NULL, "Failed to load shader loader functions.", "Rain Drops", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	HMODULE hRTM = GetModuleHandle("RealTimeMenus.dll");

	if (hRTM)
		RTM::pGetImageSpaceStage = (RTM::pfn_GetImageSpaceStage*)GetProcAddress(hRTM, "GetImageSpaceStage");
	
	if (!RTM::pGetImageSpaceStage)
		RTM::pGetImageSpaceStage = &RTM::DefaultGetImageSpaceStage;

	LoadSettings();
	InitHooks();

	return true;
}

BOOL WINAPI DllMain(
	HANDLE  hDllHandle,
	DWORD   dwReason,
	LPVOID  lpreserved
)
{
	return TRUE;
}