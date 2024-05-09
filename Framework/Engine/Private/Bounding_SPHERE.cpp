#include "..\Public\Bounding_SPHERE.h"
#include "DebugDraw.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

CBounding_SPHERE::CBounding_SPHERE()
{
}

HRESULT CBounding_SPHERE::Initialize(BOUNDING_DESC* pBoundingDesc)
{
	BOUNDING_SPHERE_DESC*	pDesc = (BOUNDING_SPHERE_DESC*)pBoundingDesc;

	m_pBoundingDesc = new BoundingSphere(pDesc->vCenter, pDesc->fRadius);	
	m_pBoundingDesc_Original = new BoundingSphere(*m_pBoundingDesc);

	return S_OK;
}

void CBounding_SPHERE::Update(_fmatrix WorldMatrix)
{
	m_pBoundingDesc_Original->Transform(*m_pBoundingDesc, WorldMatrix);
}


_bool CBounding_SPHERE::Intersect(CCollider::TYPE eType, CBounding * pTargetBounding)
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

HRESULT CBounding_SPHERE::Render(PrimitiveBatch<DirectX::VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingDesc, Compute_Color());

	return S_OK;
}

#endif

CBounding_SPHERE * CBounding_SPHERE::Create(BOUNDING_DESC* pBoundingDesc)
{
	CBounding_SPHERE* pInstance = new CBounding_SPHERE();

	if (FAILED(pInstance->Initialize(pBoundingDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_SPHERE");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_SPHERE::Free()
{
	__super::Free();
	Safe_Delete(m_pBoundingDesc_Original);
	Safe_Delete(m_pBoundingDesc);

}

