#include "..\Public\Bounding_OBB.h"
#include "DebugDraw.h"

#include "Bounding_AABB.h"
#include "Bounding_SPHERE.h"

CBounding_OBB::CBounding_OBB()
{

}

HRESULT CBounding_OBB::Initialize(BOUNDING_DESC* pBoundingDesc)
{
	BOUNDING_OBB_DESC*	pDesc = (BOUNDING_OBB_DESC*)pBoundingDesc;

	_float4		vRotation;
	XMStoreFloat4(&vRotation, XMQuaternionRotationRollPitchYaw(pDesc->vRadians.x, pDesc->vRadians.y, pDesc->vRadians.z));
	
	m_pBoundingDesc = new BoundingOrientedBox(pDesc->vCenter, pDesc->vExtents, vRotation);
	m_pBoundingDesc_Original = new BoundingOrientedBox(*m_pBoundingDesc);
	return S_OK;
}
void CBounding_OBB::Update(_fmatrix WorldMatrix)
{
	m_pBoundingDesc_Original->Transform(*m_pBoundingDesc, WorldMatrix);
}

_bool CBounding_OBB::Intersect(CCollider::TYPE eType, CBounding * pTargetBounding)
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

HRESULT CBounding_OBB::Render(PrimitiveBatch<DirectX::VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingDesc, Compute_Color());

	return S_OK;
}

#endif

CBounding_OBB * CBounding_OBB::Create(BOUNDING_DESC* pBoundingDesc)
{
	CBounding_OBB* pInstance = new CBounding_OBB();

	if (FAILED(pInstance->Initialize(pBoundingDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_OBB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_OBB::Free()
{
	__super::Free();
	Safe_Delete(m_pBoundingDesc_Original);
	Safe_Delete(m_pBoundingDesc);

}

