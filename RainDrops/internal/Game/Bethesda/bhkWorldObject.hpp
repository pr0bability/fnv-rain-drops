#pragma once

#include "bhkSerializable.hpp"
#include "CFilter.hpp"

class hkpLinkedCollidable;
class hkpWorldObject;
class hkpShape;
class bhkShape;

class bhkWorldObject : public bhkSerializable {
public:
	bhkWorldObject();
	virtual ~bhkWorldObject();

	virtual void	UpdateCollisionFilter();				// 49
	virtual void	ForceAdd(bhkWorld* apWorld);			// 50
	virtual NiNode* CreateDisplayGeometry(NiNode* apNode);	// 51
	virtual void	DestroyDisplayGeometry();				// 52

	enum Flags {
		WIND							= 1u << 0,
		DYNAMIC							= 1u << 1,
		FIXED_CONSTRAINTS				= 1u << 2,
		UNDERWATER						= 1u << 3,
		BATCH_ADD						= 1u << 4,
		BATCH_REMOVE					= 1u << 5,
		DISABLED						= 1u << 6,
		UNDER_PLACEABLE_WATER			= 1u << 7,
		STICKING_TARGET					= 1u << 8,
		KEYFRAMED_BY_LOADED_AREA_BOUND	= 1u << 9,

		IS_UNDERWATER					= UNDERWATER | UNDER_PLACEABLE_WATER,
	};

	Bitfield32 uiFlags;

	NIRTTI_ADDRESS(0x1268110);
};

ASSERT_SIZE(bhkWorldObject, 0x14)