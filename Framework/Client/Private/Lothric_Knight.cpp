#include "stdafx.h"
#include "Lothric_Knight.h"

#include "GameInstance.h"

CLothric_Knight::CLothric_Knight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CLothric_Knight::CLothric_Knight(const CLothric_Knight& rhs)
	: CGameObject(rhs)
	, m_tParams(rhs.m_tParams)
{
}

HRESULT CLothric_Knight::Initialize_Prototype()
{
	m_tParams.fMaxHp = { 590.f };
	m_tParams.fCurrentHp = { 590.f };
	m_tParams.fRigidity = { 0.0f };
	m_tParams.iSouls = { 0 };

	return S_OK;
}

HRESULT CLothric_Knight::Initialize(void* pArg)
{
	m_strObjName = TEXT("Player");

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(7.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(_float3{ 0.02f,0.02f,0.02f });

	return S_OK;
}

void CLothric_Knight::Tick(_float fTimeDelta)
{

	if (m_bIsChanging == true)
	{
		if (m_pModelCom[m_eCurrentState]->Change_Animation(*m_pCurrentChannel, fTimeDelta, 0))
		{

			m_bIsChanging = false;
		}
	}
	else
	{

	m_ePreviousState = m_eCurrentState;
	m_eCurrentState = STATE_IDLE;


		
		if (m_eCurrentState != m_ePreviousState && m_ePreviousState != STATE_IDLE)
		{
			m_pCurrentChannel = m_pModelCom[m_ePreviousState]->Get_CurrentChannels();
			m_bIsChanging = true;

		}
		else 
		{
		m_pModelCom[m_eCurrentState]->Set_Animation(0,true);
		m_pModelCom[m_eCurrentState]->Play_Animation(true, fTimeDelta);
		}
	}


}

void CLothric_Knight::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CLothric_Knight::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom[m_eCurrentState]->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (i ==5 || i ==6)
			continue;

		if (FAILED(m_pModelCom[m_eCurrentState]->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/


		if (FAILED(m_pModelCom[m_eCurrentState]->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom[m_eCurrentState]->Render(i)))
			return E_FAIL;
	}


//#pragma region 소울 출력
//
//	wsprintf(m_szSouls, TEXT("%d"), m_iSouls);
//
//	_float fLength = wcslen(m_szSouls);
//
//	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(m_szSouls), _float2(1195.f-fLength*10, 650.f), 0.5f, Colors::White);
//
//#pragma endregion

	return S_OK;
}

HRESULT CLothric_Knight::Add_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight_IDLE"),
		TEXT("Com_Model_Idle"), (CComponent**)&m_pModelCom[STATE_IDLE])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight_ToFight"),
		TEXT("Com_Model_ToFight"), (CComponent**)&m_pModelCom[STATE_TOBATTLE])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight_ToIdle"),
		TEXT("Com_Model_ToIdle"), (CComponent**)&m_pModelCom[STATE_TOIDLE])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight_Walk_Forward"),
		TEXT("Com_Model_Walk_Forward"), (CComponent**)&m_pModelCom[STATE_FORWARD])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight_Walk_Backward"),
		TEXT("Com_Model_Walk_Backward"), (CComponent**)&m_pModelCom[STATE_BACKWARD])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight_Walk_Left"),
		TEXT("Com_Model_Walk_Left"), (CComponent**)&m_pModelCom[STATE_LEFT])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight_Walk_Right"),
		TEXT("Com_Model_Walk_Right"), (CComponent**)&m_pModelCom[STATE_RIGHT])))
		return E_FAIL;

	return S_OK;
}

HRESULT CLothric_Knight::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(LEVEL_GAMEPLAY, 0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CLothric_Knight* CLothric_Knight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CLothric_Knight* pInstance = new CLothric_Knight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLothric_Knight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLothric_Knight::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CLothric_Knight* pInstance = new CLothric_Knight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLothric_Knight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLothric_Knight::Free()
{
	__super::Free();

	for (auto& pModelCom : m_pModelCom)
		Safe_Release(pModelCom);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
