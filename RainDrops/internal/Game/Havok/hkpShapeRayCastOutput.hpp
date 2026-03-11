#pragma once

#include "hkpShapeRayCastCollectorOutput.hpp"

class alignas(16) hkpShapeRayCastOutput : public hkpShapeRayCastCollectorOutput {
public:
	hkpShapeRayCastOutput();
	~hkpShapeRayCastOutput();

	uint32_t m_shapeKeys[8];
	int32_t	 m_shapeKeyIndex;

	void reset();
};

ASSERT_SIZE(hkpShapeRayCastOutput, 0x50);