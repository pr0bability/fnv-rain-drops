#pragma once

#include "hkSimdReal.hpp"

class hkTransform;
class hkQsTransform;
class hkQuaternion;

class alignas(16) hkVector4 {
public:
	hkVector4();

	explicit hkVector4(float afVal);

	hkVector4(float afX, float afY, float afZ, float afW = 0);

	hkVector4(const hkVector4& arOther);

	hkVector4(const __m128& arOther) : m_quad(arOther) {}

	__m128 m_quad;

	inline void operator= (const hkVector4& arOther) {
		m_quad = arOther.m_quad;
	}

	void setAll(float afVal);
	void set(float afX, float afY, float afZ, float afW = 0);

	operator __m128 () const { return m_quad; }
	operator __m128& () { return m_quad; }

	const float& operator[] (int32_t i) const { return m_quad.m128_f32[i]; }
	float& operator[] (int32_t i) { return m_quad.m128_f32[i]; }

	static const hkVector4 zero;
};

ASSERT_SIZE(hkVector4, 0x10);