#pragma once
#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;


public:
	HRESULT Initialize();
	void Tick();

public:
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPos, _float fRange = 0.f);

	void Transform_LocalSpace(_fmatrix WorldMatrixInv);

private:
	_float3					m_vPoints[8];
	_float3					m_vWorldPoints[8];
	_float4					m_vWorldPlanes[6];
	_float4					m_vLocalPlanes[6];
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	HRESULT Make_Plane(const _float3* pPoints, _Inout_ _float4* pPlanes);

public:
	static CFrustum* Create();
	virtual void Free() override;

};

END