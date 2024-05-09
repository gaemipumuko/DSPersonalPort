#pragma once

#include "Bounding.h"


BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct tagBoundingOBBDesc : public BOUNDING_DESC
	{
		_float3		vExtents;
		_float3		vRadians;
	}BOUNDING_OBB_DESC;

public:
	const BoundingOrientedBox* Get_BoundingDesc() const {
		return m_pBoundingDesc;
	}
	_vector Get_Center() { return XMLoadFloat3(&m_pBoundingDesc->Center); }
	_float	Get_Radius() { return max(m_pBoundingDesc->Extents.x, m_pBoundingDesc->Extents.z); }
private:
	CBounding_OBB();
	virtual ~CBounding_OBB() = default;

public:
	virtual HRESULT Initialize(BOUNDING_DESC* pBoundingDesc) override;
	virtual void Update(_fmatrix) override;
	_bool Intersect(CCollider::TYPE eType, CBounding * pTargetBounding);
#ifdef _DEBUG
	virtual HRESULT Render(PrimitiveBatch<DirectX::VertexPositionColor>* pBatch) override;
#endif
private:
	BoundingOrientedBox*			m_pBoundingDesc = { nullptr };
	BoundingOrientedBox*			m_pBoundingDesc_Original = { nullptr };

public:
	static CBounding_OBB* Create(BOUNDING_DESC* pBoundingDesc);
	virtual void Free() override;
};

END