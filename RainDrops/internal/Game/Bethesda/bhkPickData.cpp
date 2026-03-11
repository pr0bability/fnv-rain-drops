#include "bhkPickData.hpp"
#include "bhkUtilFunctions.hpp"
#include "CFilter.hpp"
#include "Gamebryo/NiPoint3.hpp"
#include "Havok/hkpClosestRayHitCollector.hpp"

// GAME - 0x4A3C20
bhkPickData::bhkPickData() {
	kLength					= hkVector4::zero;
	pCache					= nullptr;
	pClosestCollector		= nullptr;
	pAllCollector			= nullptr;
	bFailed					= false;
}

bhkPickData::~bhkPickData() {
}

// GAME - 0x7F68A0
void bhkPickData::SetLength(const NiPoint3& arLength) {
	hkVector4 kLength;
	SetLength(NI2HK(kLength, arLength));
}

// GAME - 0x4A3F90
void bhkPickData::SetLength(const hkVector4& arLength) {
	kLength = arLength;
}

// GAME - 0x9C1A20
void bhkPickData::GetLength(NiPoint3& arLength) const {
	HK2NI(arLength, kLength);
}

// GAME - 0x4A3DA0
void bhkPickData::SetFrom(const NiPoint3& arPoint) {
	hkVector4 kFrom;
	SetFrom(NI2HK(kFrom, arPoint));
}

// GAME - 0x56D340
void bhkPickData::SetFrom(const hkVector4& arPoint) {
	m_from = arPoint;
}

// GAME - 0x4585F0
void bhkPickData::GetFrom(NiPoint3& arPoint) const {
	HK2NI(arPoint, m_from);
}

// GAME - 0x4A3EB0
void bhkPickData::SetTo(const NiPoint3& arPoint) {
	hkVector4 kTo;
	SetTo(NI2HK(kTo, arPoint));
}

// GAME - 0x4A3F40
void bhkPickData::SetTo(const hkVector4& arPoint) {
	SetLength(hkVector4::zero);
	m_to = arPoint;
}

// GAME - 0x59CEB0
void bhkPickData::SetClosestCollector(hkpClosestRayHitCollector* apClosestCollector) {
	pClosestCollector = apClosestCollector;
	pAllCollector = nullptr;
}

hkpClosestRayHitCollector* bhkPickData::GetClosestCollector() const {
	return pClosestCollector;
}

// GAME - 0x4A3F70
void bhkPickData::SetFilter(const CFilter& arFilterInfo) {
	m_filterInfo = arFilterInfo.uiFilter;
}
