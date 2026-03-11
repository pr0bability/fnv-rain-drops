#pragma once

#include "Sky.hpp"
#include "TESObjectCELL.hpp"
#include "Gamebryo/NiDirectionalLight.hpp"
#include "Gamebryo/NiSourceTexture.hpp"

class TESActorBase;
class NavMeshInfoMap;
class LoadedAreaBound;
class NiFogProperty;
class TESWorldSpace;
class ImageSpaceModifierInstance;
class TESObjectREFR;
class QueuedFile;
class GridDistantArray;
class BSTempNodeManager;
class bhkPickData;
class TESRegion;
class GridCellArray;

struct DeadCountObject {
	TESActorBase*	pActorBase;
	uint16_t		uiCount;
};

class TES {
public:
	virtual bool GetMapNameForLocation(const char* arName, NiPoint3 akLocation, TESWorldSpace* apWorldSpace) const;

	GridDistantArray*									pGridDistantArray;
	GridCellArray*										pGridCellArray;
	NiNode*												pObjRoot;
	NiNode*												pLODRoot;
	NiNode*												pObjLODWaterRoot;
	BSTempNodeManager*									pTempNodeManager;
	NiDirectionalLight*									pObjLight;
	NiFogProperty*										pObjFog;
	int32_t												iCurrentGridX;
	int32_t												iCurrentGridY;
	int32_t												iCurrentQueuedX;
	int32_t												iCurrentQueuedY;
	TESObjectCELL*										pInteriorCell;
	TESObjectCELL**										pInteriorBuffer;
	TESObjectCELL**										pExteriorBuffer;
	uint32_t											uiTempInteriorBufferSize;
	uint32_t											uiTempExteriorBufferSize;
	int32_t												iSaveGridX;
	int32_t												iSaveGridY;
	bool												bCollisionBoxes;
	bool												bRunningCellTests;
	bool												bRunningCellTests2;
	uint32_t											pfnTACCallbackFunc;
	void*												pTACCallbackData;
	TESRegion*											pTACRegionFilter;
	bool												bShowLandBorders;
	uint32_t											pWaterSystem;  // TESWaterSystem*
	Sky*												pSky;
	uint32_t											kActiveImageSpaceModifiers[2];  // BSSimpleList<NiPointer<ImageSpaceModifierInstance>>
	uint32_t											uiTotalToLoad;
	uint32_t											uiLoaded;
	bool												bDisablePercentageUpdate;
	bool												bUpdateGridString;
	float												fCellDeltaX;
	float												fCellDeltaY;
	TESWorldSpace*										pWorldSpace;
	uint32_t											kLastLoadedExteriors[2];  // BSSimpleList<TESObjectCELL*>
	uint32_t											kBedsAndChairs[2];  // BSSimpleList<TESObjectREFR*>
	uint32_t											kDeadCount[2];  // BSSimpleList<DeadCountObject*>
	NiPointer<QueuedFile>								spPreloadedAddonNodes;
	NiSourceTexturePtr									spBloodDecalPreload1;
	NiPointer<QueuedFile>								spPreloadedForms;
	void*												pParticleCacheHead;
	bool												bFadeWhenLoading;
	bool												bAllowUnusedPurge;
	uint32_t											uiPlaceableWaterCount;
	NavMeshInfoMap*										pNavMeshInfoMap;
	NiPointer<LoadedAreaBound>							spLoadedAreaBound;

	static TES* GetSingleton();

	TESObjectCELL* GetCurrentCell();

	bool IsCellLoaded(const TESObjectCELL* apCell, bool abIgnoreBuffered) const;
};

ASSERT_SIZE(TES, 0xC4);