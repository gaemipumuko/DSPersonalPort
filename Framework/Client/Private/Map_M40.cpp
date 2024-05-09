#include "stdafx.h"
#include "Map_M40.h"

#include "GameInstance.h"

CMap_M40::CMap_M40(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CMap_M40::CMap_M40(const CMap_M40& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMap_M40::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap_M40::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;



	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

void CMap_M40::Tick(_float fTimeDelta)
{
	if(m_pGameInstance->Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 5.f);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * 5.f);
	}

}

void CMap_M40::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CMap_M40::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	for (size_t i = 1; i < 5; i++)
	{
		_uint		iNumMeshes = m_pModelCom[i]->Get_NumMeshes();

		for (size_t j = 0; j < iNumMeshes; j++)
		{

			if (FAILED(m_pModelCom[i]->Bind_Material(m_pShaderCom, "g_DiffuseTexture", j, aiTextureType_DIFFUSE)))
				return E_FAIL;
			/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
				return E_FAIL;*/

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom[i]->Render(j)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CMap_M40::Add_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000010"),
		TEXT("Com_Model0"), (CComponent**)&m_pModelCom[0])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000011"),
		TEXT("Com_Model1"), (CComponent**)&m_pModelCom[1])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000012"),
		TEXT("Com_Model2"), (CComponent**)&m_pModelCom[2])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000051"),
		TEXT("Com_Model3"), (CComponent**)&m_pModelCom[3])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000052"),
		TEXT("Com_Model4"), (CComponent**)&m_pModelCom[4])))
		return E_FAIL;


	return S_OK;
}

HRESULT CMap_M40::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CMap_M40* CMap_M40::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CMap_M40* pInstance = new CMap_M40(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMap_M40");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMap_M40::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CMap_M40* pInstance = new CMap_M40(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMap_M40");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMap_M40::Free()
{
	__super::Free();

	for (size_t i = 0; i < 5; i++)
	{
	Safe_Release(m_pModelCom[i]);
	}
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
