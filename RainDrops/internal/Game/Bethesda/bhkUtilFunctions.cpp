#include "bhkUtilFunctions.hpp"

#include "DirectXMath.h"

const __m128 kHK2NI = { fHK2NI, fHK2NI, fHK2NI, fHK2NI };
const __m128 kNI2HK = { fNI2HK, fNI2HK, fNI2HK, 0.f };

// GAME - 0x4A3E90
float NI2HK(float afIn) {
	return afIn * fNI2HK;
}

// GAME - 0x4A3E00
// GECK - 0x44EEE0
__m128& NI2HK(__m128& arOut, const NiPoint3& aIn) {
	arOut = DirectX::XMVectorMultiply(DirectX::XMLoadNiPoint3(aIn), kNI2HK);
	return arOut;
}

// GAME - 0x553FC0
__m128& NI2HKNS(__m128& arOut, const NiPoint3& aIn) {
	arOut = DirectX::XMLoadNiPoint3(aIn);
	return arOut;
}

// GAME - 0x4587D0
float HK2NI(float afIn) {
	return afIn * fHK2NI;
}

// GAME - 0x458620
// GECK - 0x4C79B0
NiPoint3& HK2NI(NiPoint3& arOut, const __m128& aIn) {
	DirectX::XMStoreNiPoint3(arOut, DirectX::XMVectorMultiply(aIn, kHK2NI));
	return arOut;
}

// GAME - 0x4A3970
// GECK - 0x63D2A0
NiPoint3& HK2NINS(NiPoint3& arOut, const __m128& aIn) {
	DirectX::XMStoreNiPoint3(arOut, aIn);
	return arOut;
}
