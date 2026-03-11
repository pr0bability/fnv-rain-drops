#include "DirectXUtils.hpp"
#include "d3d9.h"

#include "Gamebryo/NiTransform.hpp"
#include "Gamebryo/NiPoint4.hpp"
#include "Gamebryo/NiColor.hpp"

namespace DirectX {

	XMMATRIX XM_CALLCONV XMLoadD3DXMATRIX(const D3DXMATRIX& arSource) noexcept {
		return XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&arSource));
	}

	void XM_CALLCONV XMStoreD3DXMATRIX(D3DXMATRIX& arDest, const XMMATRIX& arSource) noexcept {
		XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadD3DXVECTOR3(const D3DXVECTOR3& arSource) noexcept {
		return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&arSource));
	}

	void XM_CALLCONV XMStoreD3DXVECTOR3(D3DXVECTOR3& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadD3DXVECTOR4(const D3DXVECTOR4& arSource) noexcept {
		return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&arSource));
	}

	void XM_CALLCONV XMStoreD3DXVECTOR4(D3DXVECTOR4& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiPoint2(const NiPoint2& arSource) noexcept {
		return XMLoadFloat2(reinterpret_cast<const XMFLOAT2*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiPoint2(NiPoint2& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiPoint3(const NiPoint3& arSource) noexcept {
		return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiPoint3(NiPoint3& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiPoint4(const NiPoint4& arSource) noexcept {
		return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiPoint4(NiPoint4& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiBound(const NiBound& arSource) noexcept {
		return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiBound(NiBound& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiPlane(const NiPlane& arSource) noexcept {
		return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiPlane(NiPlane& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&arDest), arSource);
	}

	XMMATRIX XM_CALLCONV XMLoadNiMatrix3(const NiMatrix3& arSource) noexcept {
		return XMLoadFloat3x3(reinterpret_cast<const XMFLOAT3X3*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiMatrix3(NiMatrix3& arDest, const XMMATRIX& arSource) noexcept {
		XMStoreFloat3x3(reinterpret_cast<XMFLOAT3X3*>(&arDest), arSource);
	}

	void XM_CALLCONV XMLoadNiTransform(const NiTransform& arSource, XMMATRIX& arRot, XMVECTOR& arTrans, float& afScale) noexcept {
		arRot = XMLoadNiMatrix3(arSource.m_Rotate);
		arTrans = XMLoadNiPoint3(arSource.m_Translate);
		afScale = arSource.m_fScale;
	}

	void XM_CALLCONV XMLoadNiTransform(const NiTransform& arSource, XMMATRIX& arRot, XMVECTOR& arTrans, XMVECTOR& afScale) noexcept {
		arRot = XMLoadNiMatrix3(arSource.m_Rotate);
		arTrans = XMLoadNiPoint3(arSource.m_Translate);
		afScale = XMVectorReplicate(arSource.m_fScale);
	}

	void XM_CALLCONV XMStoreNiTransform(NiTransform& arDest, const XMMATRIX& arRot, const XMVECTOR& arTrans, const float& afScale) noexcept {
		DirectX::XMStoreNiPoint3(arDest.m_Translate, arTrans);
		DirectX::XMStoreNiMatrix3(arDest.m_Rotate, arRot);
		arDest.m_fScale = afScale;
	}

	void XM_CALLCONV XMStoreNiTransform(NiTransform& arDest, const XMMATRIX& arRot, const XMVECTOR& arTrans, const XMVECTOR& afScale) noexcept {
		DirectX::XMStoreNiPoint3(arDest.m_Translate, arTrans);
		DirectX::XMStoreNiMatrix3(arDest.m_Rotate, arRot);
		DirectX::XMStoreFloat(&arDest.m_fScale, afScale);
	}

	XMVECTOR XM_CALLCONV XMLoadNiColor(const NiColor& arSource) noexcept {
		return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiColor(NiColor& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiColorA(const NiColorA& arSource) noexcept {
		return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiColorA(NiColorA& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiQuaternion(const NiQuaternion& arSource) noexcept {
		return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&arSource));
	}

	void XM_CALLCONV XMStoreNiQuaternion(NiQuaternion& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&arDest), arSource);
	}

	XMVECTOR XM_CALLCONV XMLoadNiQuaternionSwizzle(const NiQuaternion& arSource) noexcept {
		return XMVectorSwizzle(XMLoadNiQuaternion(arSource), 1, 2, 3, 0);
	}

	void XM_CALLCONV XMStoreNiQuaternionSwizzle(NiQuaternion& arDest, const XMVECTOR& arSource) noexcept {
		XMStoreNiQuaternion(arDest, XMVectorSwizzle(arSource, 3, 0, 1, 2));
	}

	XMVECTOR XM_CALLCONV XMMatrixTransform(const XMMATRIX& arMatrix, const XMVECTOR& pt) noexcept {
		float fX = XMVectorGetX(XMVector3Dot(arMatrix.r[0], pt));
		float fY = XMVectorGetX(XMVector3Dot(arMatrix.r[1], pt));
		float fZ = XMVectorGetX(XMVector3Dot(arMatrix.r[2], pt));
		return XMVectorSet(fX, fY, fZ, 0.f);
	}

	// Based on https://stackoverflow.com/questions/32408665/fastest-way-to-compute-absolute-value-using-sse
	__m128 abs_mask() noexcept {
		__m128i minus1 = _mm_set1_epi32(-1);
		return _mm_castsi128_ps(_mm_srli_epi32(minus1, 1));
	}

	XMVECTOR XM_CALLCONV XMVectorAbsFast(const XMVECTOR& pt) noexcept {
		return _mm_and_ps(abs_mask(), pt);
	}
}