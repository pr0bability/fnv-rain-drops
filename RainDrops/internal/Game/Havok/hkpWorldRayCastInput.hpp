#pragma once

#include "hkVector4.hpp"

class hkpWorldRayCastInput {
public:
	hkpWorldRayCastInput();
	~hkpWorldRayCastInput();

	hkVector4	m_from;
	hkVector4	m_to;
	bool		m_enableShapeCollectionFilter;
	uint32_t	m_filterInfo;
};

ASSERT_SIZE(hkpWorldRayCastInput, 0x30);