#include "stdafx.h"
#include "Untended_Pick.h"

#include "GameInstance.h"

CUntended_Pick::CUntended_Pick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUntended_Pick::CUntended_Pick(const CUntended_Pick& rhs)
	: CGameObject(rhs)
{
}

HRESULT CUntended_Pick::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUntended_Pick::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT,XMVectorSet(0.9986f,0.f,0.524f,0.f));
	//m_pTransformCom->Set_State(CTransform::STATE_UP,XMVectorSet(0.f,1.f,0.f,0.f));
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK,XMVectorSet(-0.0524699837f,0.f,.99862211f,0.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMVectorSet(-49.3474960f,27.0063667f,-53.9166031,1.f) );
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -300.0063667f, 0.f, 1.f));
	return S_OK;
}

void CUntended_Pick::Tick(_float fTimeDelta)
{
	//if (m_pGameInstance->Get_DIKeyState(DIK_Z) & 0x80)
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 2.f);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_X) & 0x80)
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * 2.f);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_F) & 0x80)
	//{
	//	m_pTransformCom->Go_Straight(fTimeDelta);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_V) & 0x80)
	//{
	//	m_pTransformCom->Go_Backward(fTimeDelta);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_C) & 0x80)
	//{
	//	m_pTransformCom->Go_Left(fTimeDelta);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_B) & 0x80)
	//{
	//	m_pTransformCom->Go_Right(fTimeDelta);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_G) & 0x80)
	//{
	//	m_pTransformCom->Go_Up(fTimeDelta);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_H) & 0x80)
	//{
	//	m_pTransformCom->Go_Down(fTimeDelta);
	//}


}

void CUntended_Pick::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CUntended_Pick::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CUntended_Pick::Add_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Model_Map_Gundyr_Pick"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUntended_Pick::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CUntended_Pick* CUntended_Pick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CUntended_Pick* pInstance = new CUntended_Pick(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUntended_Pick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUntended_Pick::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CUntended_Pick* pInstance = new CUntended_Pick(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUntended_Pick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUntended_Pick::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
