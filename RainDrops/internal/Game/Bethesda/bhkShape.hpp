#pragma once

#include "bhkSerializable.hpp"
#include "Gamebryo/NiColor.hpp"

class bhkShapeCollection;
class hkpShape;
class hkGeometry;

class bhkShape : public bhkSerializable {
public:
	virtual void				CopyMembers(bhkSerializable* apDestination, NiCloningProcess* apCloneProc);												// 49
	virtual bool				CalcMass(void* apMassProperty);																							// 50
	virtual bhkShapeCollection* FindShapeCollection(); 																									// 51
	virtual bool				CanShare(NiCloningProcess* apCloneProc);																				// 52
	virtual void				BuildDisplayGeometry(NiNode* apNode, const NiColor* apColor);															// 53
	virtual void				DestroyDisplayGeometry();																								// 54
	virtual void				BuildDisplayFromhkGeometry(NiNode* apNode, const hkGeometry* apGeometry, const NiColor* apColor, const char* apName);	// 55

	Bitfield32 uiMaterialAndFlags;
};

ASSERT_SIZE(bhkShape, 0x14);