#pragma once

#include "Havok/hkVector4.hpp"
#include "Havok/hkpWorldRayCastInput.hpp"
#include "Havok/hkpWorldRayCastOutput.hpp"

class hkpAllRayHitCollector;
class hkpClosestRayHitCollector;
class NiAVObject;
class hkpRigidBody;
class hkpPhantom;
class CFilter;

class bhkPickData : public hkpWorldRayCastInput, public hkpWorldRayCastOutput {
public:
	bhkPickData();
	~bhkPickData();

	hkVector4					kLength;
	char*						pCache;
	hkpClosestRayHitCollector*	pClosestCollector;
	hkpAllRayHitCollector*		pAllCollector;
	bool						bFailed;

	void SetLength(const NiPoint3& arLength);
	void SetLength(const hkVector4& arLength);
	void GetLength(NiPoint3& arLength) const;

	void SetFrom(const NiPoint3& arPoint);
	void SetFrom(const hkVector4& arPoint);
	void GetFrom(NiPoint3& arPoint) const;

	void SetTo(const NiPoint3& arPoint);
	void SetTo(const hkVector4& arPoint);

	void SetFilter(const CFilter& arFilterInfo);

	void SetClosestCollector(hkpClosestRayHitCollector* apClosestCollector);
	hkpClosestRayHitCollector* GetClosestCollector() const;
};

ASSERT_SIZE(bhkPickData, 0xB0);