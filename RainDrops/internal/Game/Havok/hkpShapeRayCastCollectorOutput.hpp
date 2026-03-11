#pragma once

#include "hkVector4.hpp"

class alignas(16) hkpShapeRayCastCollectorOutput {
public:
	hkpShapeRayCastCollectorOutput();
	~hkpShapeRayCastCollectorOutput();

	hkVector4	m_normal;
	float		m_hitFraction;
	int32_t		m_extraInfo;
};

ASSERT_SIZE(hkpShapeRayCastCollectorOutput, 0x20);