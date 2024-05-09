#include "..\Public\Bounding_AABB.h"
#include "DebugDraw.h"

#include "Bounding_OBB.h"
#include "Bounding_SPHERE.h"



CBounding_AABB::CBounding_AABB()
{
}

HRESULT CBounding_AABB::Initialize(BOUNDING_DESC* pBoundingDesc)
{
	BOUNDING_AABB_DESC*	pDesc = (BOUNDING_AABB_DESC*)pBoundingDesc;

	m_pBoundingDesc = new BoundingBox(pDesc->vCenter, pDesc->vExtents);
	m_pBoundingDesc_Original = new BoundingBox(*m_pBoundingDesc);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix WorldMatrix)
{
	_matrix		TransformMatrix = WorldMatrix;


	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	m_pBoundingDesc_Original->Transform(*m_pBoundingDesc, TransformMatrix);
}

_bool CBounding_AABB::Intersect(CCollider::TYPE eType, CBounding * pTargetBounding)
{
	m_isColl = false;

	switch (eType)
	{
	case CCollider::TYPE_AABB:
		m_isColl = m_pBoundingDesc->Intersects(*((CBounding_AABB*)pTargetBounding)->Get_BoundingDesc());
		break;
	case CCollider::TYPE_OBB:
		m_isColl = m_pBoundingDesc->Intersects(*((CBounding_OBB*)pTargetBounding)->Get_BoundingDesc());
		break;
	case CCollider::TYPE_SPHERE:
		m_isColl = m_pBoundingDesc->Intersects(*((CBounding_SPHERE*)pTargetBounding)->Get_BoundingDesc());
		break;	
	}

	return m_isColl;
}

#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<DirectX::VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingDesc, Compute_Color());
	
	return S_OK;
}

#endif

CBounding_AABB * CBounding_AABB::Create(BOUNDING_DESC* pBoundingDesc)
{
	CBounding_AABB* pInstance = new CBounding_AABB();

	if (FAILED(pInstance->Initialize(pBoundingDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pBoundingDesc_Original);
	Safe_Delete(m_pBoundingDesc);
}

