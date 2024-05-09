#pragma once

#include "Bounding.h"


BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagBoundingAABBDesc : public BOUNDING_DESC
	{
		_float3		vExtents;
	}BOUNDING_AABB_DESC;
private:
	CBounding_AABB();
	virtual ~CBounding_AABB() = default;

public:
	const BoundingBox* Get_BoundingDesc() const {
		return m_pBoundingDesc;
	}
	_vector Get_Center() { return XMLoadFloat3(&m_pBoundingDesc->Center); }
	_float	Get_Radius() { return max(m_pBoundingDesc->Extents.x, m_pBoundingDesc->Extents.z); }

public:
	virtual HRESULT Initialize(BOUNDING_DESC* pBoundingDesc) override;
	virtual void Update(_fmatrix) override;
	virtual _bool Intersect(CCollider::TYPE eType, CBounding* pTargetBounding) override;
#ifdef _DEBUG
	virtual HRESULT Render(PrimitiveBatch<DirectX::VertexPositionColor>* pBatch) override;
#endif
private:
	BoundingBox*			m_pBoundingDesc_Original = { nullptr };
	BoundingBox*			m_pBoundingDesc = { nullptr };

public:
	static CBounding_AABB* Create(BOUNDING_DESC* pBoundingDesc);
	virtual void Free() override;
};

END