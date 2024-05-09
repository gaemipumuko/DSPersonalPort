#include "stdafx.h"
#include "..\Public\LandObject.h"

CLandObject::CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
	
}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void* pArg)
{
	return S_OK;
}

void CLandObject::Tick(_float fTimeDelta)
{
	


}

void CLandObject::LateTick(_float fTimeDelta)
{

}

HRESULT CLandObject::Render()
{
	return S_OK;
}

HRESULT CLandObject::SetUp_OnTerrain(CTransform* pTransform, _float fRevisionY)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CVIBuffer_Terrain*	pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	CTransform*	pTerrainTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform"));

	_vector		vWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

	//vWorldPos = XMVectorSetY(vWorldPos, fRevisionY + pTerrainBuffer->Compute_Height(vWorldPos, pTerrainTransform));

	pTransform->Set_State(CTransform::STATE_POSITION, vWorldPos);

	Safe_Release(pGameInstance);

	return S_OK;	
}
//
//HRESULT CLandObject::LookAt(CTransform * pTransform, const _float3 & vTargetPos)
//{
//	pTransform->LookAt(vTargetPos);
//
//	_float3		vScale = pTransform->Get_Scale();
//
//	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
//
//	_float3		vRight = pTransform->Get_State(CTransform::STATE_RIGHT);
//
//	_float3		vLook;
//
//	vLook = *D3DXVec3Normalize(&vLook, D3DXVec3Cross(&vLook, &vRight, &vUp)) * vScale.z;
//
//	pTransform->Set_State(CTransform::STATE_UP, vUp);
//	pTransform->Set_State(CTransform::STATE_LOOK, vLook);
//
//	return S_OK;
//}

void CLandObject::Free()
{
	__super::Free();

}
