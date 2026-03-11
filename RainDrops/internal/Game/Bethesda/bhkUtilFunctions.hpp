#pragma once

class NiPoint3;
class NiMatrix3;
class NiTransform;
class NiQuaternion;

extern const __m128 kHK2NI;
extern const __m128 kNI2HK;

// To Havok

extern float NI2HK(float afIn);

extern __m128& NI2HK(__m128& arOut, const NiPoint3& aIn);

extern __m128& NI2HKNS(__m128& arOut, const NiPoint3& aIn);

// From Havok

extern float HK2NI(float afIn);

extern NiPoint3& HK2NI(NiPoint3& arOut, const __m128& aIn);

extern NiPoint3& HK2NINS(NiPoint3& arOut, const __m128& aIn);
