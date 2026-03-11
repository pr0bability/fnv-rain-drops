#pragma once

#include "bhkSerializable.hpp"
#include "Havok/hkVector4.hpp"
#include "Gamebryo/NiAVObject.hpp"

class bhkNiCollisionObject;
class TESTrapListener;
class TESWaterListener;
class BGSAcousticSpaceListener;
class hkpConstraintInstance;
class bhkEntityListener;
class bhkPickData;
class hkpAction;
class hkpPhantom;
class hkpEntity;
class hkpCollidable;
class hkVisualDebugger;

class alignas(16) bhkWorld : public bhkSerializable {
public:
	virtual void Update();
	virtual bool PickObject(bhkPickData& arPickData);
	virtual bool HasActiveSimIslands();
	virtual void AddObjects(NiAVObject* apObject, bool abRecurse, bool abNotify, int aiGroup, bool abForce);
	virtual void AddObjectsAndMoveController(NiAVObject* apObject, NiAVObject* apRoot);
	virtual void SetDebugDisplay(void*, bool abTF);

	enum ObjectAction {
		OA_ADD					= 0,
		OA_REMOVE				= 1,
		OA_NI2HK				= 2,
		OA_ENABLE				= 3,
		OA_KILL_HAVOK			= 4,
		OA_SET_MOTION			= 5,
		OA_DEBUG_DISPLAY		= 6,
		OA_SET_GROUP			= 7,
		OA_GET_ACTIVE			= 8,
		OA_CHANGE_LAYER			= 9,
		OA_FIXED_TO_KEYFRAMED	= 10,
		OA_SHIFT_POS			= 11,
		OA_ACTIVATE				= 12,
		OA_FORCE_ADD			= 13,
		OA_ENABLECOLLISION		= 14,
		OA_SET_RESTITUTION		= 15,
		OA_SET_FRICTION			= 16,
		OA_DISMEMBER			= 17,
		OA_TOTAL,
	};

	struct ObjectRecData {
		bhkWorld*	pWorld;
		bool		bRecurse;
		uint32_t	eAction;
		uint32_t	uData[4];
	};

	struct DebugColor {
		hkpCollidable*	pCollidable;
		uint32_t		uiColor;
	};

	void*		pDebugger;
	bool		bAdded;
	bool		bEnabled;
	uint32_t	pad[32];

	static constexpr AddressPtr<float, 0x11AFE68> fBSDeltaTime;
};

ASSERT_SIZE(bhkWorld, 0xA0);