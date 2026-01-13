#pragma once

#include "Gamebryo/NiColor.hpp"
#include "SkyTextureManager.hpp"

class Atmosphere;
class Clouds;
class Sun;
class Stars;
class Precipitation;
class Moon;
class TESClimate;
class TESWeather;
class ImageSpaceModifierInstanceForm;

class Sky {
public:
	Sky();
	virtual ~Sky();

	enum SkyObjectType {
		SOT_SUNGLARE = 0x1,
		SOT_CLOUDS = 0x3,
		SOT_STARS = 0x5,
		SOT_MOON = 0x7,
	};

	enum SkyColors {
		SC_SKY_UPPER	= 0,
		SC_FOG			= 1,
		SC_CLOUDS_LOWER = 2,
		SC_AMBIENT		= 3,
		SC_SUNLIGHT		= 4,
		SC_SUN			= 5,
		SC_STARS		= 6,
		SC_SKY_LOWER	= 7,
		SC_HORIZON		= 8,
		SC_CLOUDS_UPPER = 9,
		SC_COUNT,
	};

	enum FogDistance : __int32 {
		FOG_NEAR			= 0,
		FOG_FAR				= 1,
		FOG_WATER_NEAR		= 2,
		FOG_WATER_FAR		= 3,
		FOG_NUM_DISTANCES	= 4,
	};

	enum SKY_MODE {
		SM_NONE			= 0,
		SM_INTERIOR		= 1,
		SM_SKYDOME_ONLY = 2,
		SM_FULL			= 3,
	};

	enum SkyFlags {
		UNDERWATER				= 0x4,
		FAST_TRAVEL				= 0x10,
		REFRESH_MOON			= 0x20,
		REFRESH_CLIMATE			= 0x40,
		HIDE_SKY				= 0x80,
		REFRESH_SUNRISE_BEGIN	= 0x100,
		REFRESH_SUNRISE_END		= 0x200,
		REFRESH_SUNSET_BEGIN	= 0x400,
		REFRESH_SUNSET_END		= 0x800,
		REFRESH_SUNRISE_COLOR	= 0x1000,
		REFRESH_SUNSET_COLOR	= 0x2000,
	};

	struct SkySound {
		uint32_t		kHandle[3];  // BSSoundHandle
		TESWeather*		pWeather;
		uint32_t		uiType;
		uint32_t		uiFormID;
		uint32_t		uiData;
	};

	struct COLOR_BLEND {
		uint32_t	uiRGBVal[4];
		float		fBlend[4];
	};

	uint32_t						spRoot;
	uint32_t						spMoonsRoot;
	TESClimate*						pCurrentClimate;
	TESWeather*						pCurrentWeather;
	TESWeather*						pLastWeather;
	TESWeather*						pDefaultWeather;
	TESWeather*						pOverrideWeather;
	Atmosphere*						pAtmosphere;
	Stars*							pStars;
	Sun*							pSun;
	Clouds*							pClouds;
	Moon*							pMasser;
	Moon*							pSecunda;
	Precipitation*					pPrecipitation;
	NiColor							kColors[SC_COUNT];
	NiColor							kWaterFogColor;
	NiColor							kSunSpecularColor;
	float							fWindSpeed;
	float							fWindAngle;
	float							fFogDistances[FOG_NUM_DISTANCES];
	float							fFogHeight;
	float							fFogPower;
	float							fCurrentGameHour;
	float							fLastWeatherUpdate;
	float							fCurrentWeatherPct;
	SKY_MODE						eMode;
	uint32_t						pSkySoundList;
	float							fFlash;
	uint32_t						uiFlashTime;
	uint32_t						uiLastMoonPhaseUpdate;
	float							fWindowReflectionTimer;
	float							fAccelBeginPct;
	SkyTextureManager				kTextureManager;
	Bitfield32						uiFlags;
	ImageSpaceModifierInstanceForm* pFadeInIMODCurrent;
	ImageSpaceModifierInstanceForm* pFadeOutIMODCurrent;
	ImageSpaceModifierInstanceForm* pFadeInIMODLast;
	ImageSpaceModifierInstanceForm* pFadeOutIMODLast;
	float							fHighNoon;
	float							fMidnightWax;
	float							fMidnightWane;

	static Sky* GetSingleton();

	bool GetIsRaining() const;
	bool GetIsSnowing() const;

	static bool IsUnderwater();
};

ASSERT_SIZE(Sky, 0x138);