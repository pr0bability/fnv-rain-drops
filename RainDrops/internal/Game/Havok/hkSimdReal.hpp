#pragma once

class hkSimdReal {
public:
	hkSimdReal(const __m128& x) : m_real(x) {}
	hkSimdReal(float f) { m_real = _mm_load_ss(&f); }

	__m128 m_real;

	operator float() const {
		float f;
		_mm_store_ss(&f, m_real);
		return f;
	}

	__m128 broadcast() const {
		return _mm_shuffle_ps(m_real, m_real, 0);
	}

	const __m128& getQuad() const {
		return m_real;
	}
};

ASSERT_SIZE(hkSimdReal, 0x10);