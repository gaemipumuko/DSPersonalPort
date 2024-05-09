#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_SPHERE final : public CBounding
{
public:
	typedef struct tagBoundingSPHEREDesc : public BOUNDING_DESC
	{
		_float		fRadius;
	}BOUNDING_SPHERE_DESC;
private:
	CBounding_SPHERE();
	virtual ~CBounding_SPHERE() = default;

public:
	const BoundingSphere* Get_BoundingDesc() const {
		return m_pBoundingDesc;
	}
	_vector Get_Center() { return XMLoadFloat3(&m_pBoundingDesc->Center); }
	_float Get_Radius() { return m_pBoundingDesc->Radius; }

public:
	virtual HRESULT Initialize(BOUNDING_DESC* pBoundingDesc) override;
	virtual void Update(_fmatrix) override;
	_bool Intersect(CCollider::TYPE eType, CBounding * pTargetBounding);
#ifdef _DEBUG
	virtual HRESULT Render(PrimitiveBatch<DirectX::VertexPositionColor>* pBatch) override;
#endif
private:
	BoundingSphere*			m_pBoundingDesc = { nullptr };
	BoundingSphere*			m_pBoundingDesc_Original = { nullptr };

public:
	static CBounding_SPHERE* Create(BOUNDING_DESC* pBoundingDesc);
	virtual void Free() override;
};

END