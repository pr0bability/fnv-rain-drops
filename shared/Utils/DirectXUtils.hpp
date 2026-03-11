#pragma once

#include "DirectXMath.h"

class NiPoint2;
class NiPoint3;
class NiPoint4;
class NiColor;
class NiColorA;
class NiMatrix3;
class NiBound;
class NiTransform;
class NiPlane;
class NiQuaternion;
class bhkVector4;

namespace DirectX {
	XMMATRIX XM_CALLCONV XMLoadD3DXMATRIX(const D3DXMATRIX& arSource) noexcept;

	void XM_CALLCONV XMStoreD3DXMATRIX(D3DXMATRIX& arDest, const XMMATRIX& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadD3DXVECTOR3(const D3DXVECTOR3& arSource) noexcept;

	void XM_CALLCONV XMStoreD3DXVECTOR3(D3DXVECTOR3& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadD3DXVECTOR4(const D3DXVECTOR4& arSource) noexcept;

	void XM_CALLCONV XMStoreD3DXVECTOR4(D3DXVECTOR4& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiPoint2(const NiPoint2& arSource) noexcept;

	void XM_CALLCONV XMStoreNiPoint2(NiPoint2& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiPoint3(const NiPoint3& arSource) noexcept;

	void XM_CALLCONV XMStoreNiPoint3(NiPoint3& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiPoint4(const NiPoint4& arSource) noexcept;

	void XM_CALLCONV XMStoreNiPoint4(NiPoint4& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiBound(const NiBound& arSource) noexcept;

	void XM_CALLCONV XMStoreNiBound(NiBound& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiPlane(const NiPlane& arSource) noexcept;

	void XM_CALLCONV XMStoreNiPlane(NiPlane& arDest, const XMVECTOR& arSource) noexcept;

	XMMATRIX XM_CALLCONV XMLoadNiMatrix3(const NiMatrix3& arSource) noexcept;

	void XM_CALLCONV XMStoreNiMatrix3(NiMatrix3& arDest, const XMMATRIX& arSource) noexcept;

	void XM_CALLCONV XMLoadNiTransform(const NiTransform& arSource, XMMATRIX& arRot, XMVECTOR& arTrans, float& afScale) noexcept;
	void XM_CALLCONV XMLoadNiTransform(const NiTransform& arSource, XMMATRIX& arRot, XMVECTOR& arTrans, XMVECTOR& afScale) noexcept;

	void XM_CALLCONV XMStoreNiTransform(NiTransform& arDest, const XMMATRIX& arRot, const XMVECTOR& arTrans, const float& afScale) noexcept;
	void XM_CALLCONV XMStoreNiTransform(NiTransform& arDest, const XMMATRIX& arRot, const XMVECTOR& arTrans, const XMVECTOR& afScale) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiColor(const NiColor& arSource) noexcept;

	void XM_CALLCONV XMStoreNiColor(NiColor& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiColorA(const NiColorA& arSource) noexcept;

	void XM_CALLCONV XMStoreNiColorA(NiColorA& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiQuaternion(const NiQuaternion& arSource) noexcept;

	void XM_CALLCONV XMStoreNiQuaternion(NiQuaternion& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadbhkVector4(const bhkVector4& arSource) noexcept;

	void XM_CALLCONV XMStorebhkVector4(bhkVector4& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMLoadNiQuaternionSwizzle(const NiQuaternion& arSource) noexcept;

	void XM_CALLCONV XMStoreNiQuaternionSwizzle(NiQuaternion& arDest, const XMVECTOR& arSource) noexcept;

	XMVECTOR XM_CALLCONV XMMatrixTransform(const XMMATRIX& arMatrix, const XMVECTOR& pt) noexcept;

	XMVECTOR XM_CALLCONV XMVectorAbsFast(const XMVECTOR& pt) noexcept;
}