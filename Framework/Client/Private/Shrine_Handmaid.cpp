#include "stdafx.h"
#include "Shrine_Handmaid.h"

#include "GameInstance.h"

CHandMaid::CHandMaid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CHandMaid::CHandMaid(const CHandMaid& rhs)
	: CGameObject(rhs)
	, m_tParams(rhs.m_tParams)
{
}

HRESULT CHandMaid::Initialize_Prototype()
{
	m_tParams.fMaxHp = { 590.f };
	m_tParams.fCurrentHp = { 590.f };
	m_tParams.fRigidity = { 0.0f };
	m_tParams.iSouls = { 0 };

	return S_OK;
}

HRESULT CHandMaid::Initialize(void* pArg)
{
	m_strObjName = TEXT("Fire_Keeper");

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 40.f, 1.f));

	return S_OK;
}

void CHandMaid::Tick(_float fTimeDelta)
{

	CTransform* pPlayer_Transform = (CTransform*)m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pPlayer_Transform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = vPlayerPos - vPosition;
	
	XMVector3Length(vToPlayer);
	XMVector3Normalize(vToPlayer);
	if (XMVector3Dot(XMVector3Normalize(vToPlayer), XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK))).m128_f32[0] < 0 &&
		XMVector3Length(vToPlayer).m128_f32[0] <= 5.f )
	{
		if (m_bIsApproched)
		{
			wstring strOK = TEXT(": 대화하기");
		m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_Interaction"),&strOK);
		m_bIsApproched = false;
		}

	}
	//if (XMVector3Dot(XMVector3Normalize(vToPlayer), XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK))))
	//{

	//}

	/*if (m_bIsChanging == true)
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


		if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
		{
			m_pTransformCom->Go_Straight(fTimeDelta);
			m_eCurrentState = STATE_FORWARD;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
		{
			m_pTransformCom->Go_Backward(fTimeDelta);
			m_eCurrentState = STATE_BACKWARD;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		{
			m_eCurrentState = STATE_LEFT;
			m_pTransformCom->Go_Left(fTimeDelta);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		{
			m_eCurrentState = STATE_RIGHT;
			m_pTransformCom->Go_Right(fTimeDelta);
		}


		if (m_eCurrentState != m_ePreviousState && m_ePreviousState != STATE_IDLE)
		{
			m_pCurrentChannel = m_pModelCom[m_ePreviousState]->Get_CurrentChannels();
			m_bIsChanging = true;

		}
		else 
		{*/
		m_pModelCom[m_eCurrentState]->Set_Animation(0,true);
		m_pModelCom[m_eCurrentState]->Play_Animation(true, fTimeDelta);
	//	}
	//}


}

void CHandMaid::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CHandMaid::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom[m_eCurrentState]->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{

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

HRESULT CHandMaid::Add_Component()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper_IDLE"),
		TEXT("Com_Model_Idle"), (CComponent**)&m_pModelCom[STATE_IDLE])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper_GiveBuff"),
		TEXT("Com_Model_GiveBuff"), (CComponent**)&m_pModelCom[STATE_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper_GiveBuff_While"),
		TEXT("Com_Model_Buff_While"), (CComponent**)&m_pModelCom[STATE_BUFF_WHILE])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper_GiveBuff_End"),
		TEXT("Com_Model_Buff_End"), (CComponent**)&m_pModelCom[STATE_BUFF_END])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper_Salute"),
		TEXT("Com_Model_Walk_Salute"), (CComponent**)&m_pModelCom[STATE_SALUTE])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper_Shy"),
		TEXT("Com_Model_Shy"), (CComponent**)&m_pModelCom[STATE_SHY])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper_GraceTurn"),
		TEXT("Com_Model_Turn"), (CComponent**)&m_pModelCom[STATE_TURN])))
		return E_FAIL;

	return S_OK;
}

HRESULT CHandMaid::Bind_ShaderResources()
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

CHandMaid* CHandMaid::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CHandMaid* pInstance = new CHandMaid(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHandMaid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHandMaid::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CHandMaid* pInstance = new CHandMaid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHandMaid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHandMaid::Free()
{
	__super::Free();

	for (auto& pModelCom : m_pModelCom)
		Safe_Release(pModelCom);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
