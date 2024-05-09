#pragma once

#include "Collider.h"

/* aabb, obb, sphere의 부모가 되는 클래스다 .*/

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoundingDesc
	{
		_float3		vCenter;
	}BOUNDING_DESC;
protected:
	CBounding();
	virtual ~CBounding() = default;

public:
	virtual HRESULT Initialize(BOUNDING_DESC* pBoundingDesc) { return S_OK; }
	virtual void Update(_fmatrix) = 0;
	virtual _bool Intersect(CCollider::TYPE eType, CBounding* pTargetBounding) = 0;

	virtual _vector Get_Center() = 0;
	virtual _float Get_Radius() = 0;
#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<DirectX::VertexPositionColor>* pBatch) = 0;
#endif
protected:
	_bool			m_isColl = { false };

#ifdef _DEBUG
protected:
	_vector		Compute_Color();
#endif


public:	
	virtual void Free() override;
};

END