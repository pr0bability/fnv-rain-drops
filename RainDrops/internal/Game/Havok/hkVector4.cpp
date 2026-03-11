#include "hkVector4.hpp"

const hkVector4 hkVector4::zero = hkVector4(0.0f);

// GAME - 0x6815C0
hkVector4::hkVector4() {
}

hkVector4::hkVector4(float afVal) {
	setAll(afVal);
}

// GAME - 0x5532A0
hkVector4::hkVector4(float afX, float afY, float afZ, float afW) {
	set(afX, afY, afZ, afW);
}

// GAME - 0x4A3C90
hkVector4::hkVector4(const hkVector4& arOther) {
	m_quad = arOther.m_quad;
}

// GAME - 0x4589C0
void hkVector4::setAll(float afVal) {
	m_quad = DirectX::XMVectorReplicate(afVal);
}

// GAME - 0x4B4D50
void hkVector4::set(float afX, float afY, float afZ, float afW) {
	m_quad = DirectX::XMVectorSet(afX, afY, afZ, afW);
}
