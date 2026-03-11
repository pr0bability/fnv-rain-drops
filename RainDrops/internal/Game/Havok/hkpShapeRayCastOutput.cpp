#include "hkpShapeRayCastOutput.hpp"

// GAME - 0x4A3D20
hkpShapeRayCastOutput::hkpShapeRayCastOutput() {
	reset();
}

hkpShapeRayCastOutput::~hkpShapeRayCastOutput() {
}

// GAME - 0x4A3D80
void hkpShapeRayCastOutput::reset() {
	m_shapeKeyIndex = 0;
	m_shapeKeys[0] = UINT32_MAX;
}
