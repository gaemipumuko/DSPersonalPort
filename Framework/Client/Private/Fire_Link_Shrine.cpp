#include "stdafx.h"
#include "Fire_Link_Shrine.h"

#include "GameInstance.h"

CFireLinkShrine::CFireLinkShrine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CFireLinkShrine::CFireLinkShrine(const CFireLinkShrine& rhs)
	: CGameObject(rhs)
{
}

HRESULT CFireLinkShrine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFireLinkShrine::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT,XMVectorSet(-0.309195f,0.f,-0.95099f,0.f));
	m_pTransformCom->Set_State(CTransform::STATE_UP,XMVectorSet(0.f,1.f,0.f,0.f));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK,XMVectorSet(0.9509986f,0.f,-0.309195369f,0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMVectorSet(-18.3357716f,-8.83692741f,9.50086689f,1.f) );


	return S_OK;
}

void CFireLinkShrine::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_Z) & 0x80)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 2.f);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_X) & 0x80)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * 2.f);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_F) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_V) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_C) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_B) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_G) & 0x80)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_H) & 0x80)
	{
		m_pTransformCom->Go_Down(fTimeDelta);
	}


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vector{ 200.f,100.f,200.f,1.f });

}

void CFireLinkShrine::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CFireLinkShrine::Render()
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

HRESULT CFireLinkShrine::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Model_Fire_Link_Shrine"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFireLinkShrine::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CFireLinkShrine* CFireLinkShrine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CFireLinkShrine* pInstance = new CFireLinkShrine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFireLinkShrine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFireLinkShrine::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CFireLinkShrine* pInstance = new CFireLinkShrine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFireLinkShrine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFireLinkShrine::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
