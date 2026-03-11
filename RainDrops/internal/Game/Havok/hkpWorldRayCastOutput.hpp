#pragma once

#include "hkpShapeRayCastOutput.hpp"

class hkpCollidable;

class alignas(16) hkpWorldRayCastOutput : public hkpShapeRayCastOutput {
public:
	hkpWorldRayCastOutput();
	~hkpWorldRayCastOutput();

	hkpCollidable* m_rootCollidable;
};

ASSERT_SIZE(hkpWorldRayCastOutput, 0x60);