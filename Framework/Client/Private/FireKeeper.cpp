#include "stdafx.h"
#include "FireKeeper.h"
#include "Sound_Manager.h"

#include "GameInstance.h"

CFire_Keeper::CFire_Keeper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CFire_Keeper::CFire_Keeper(const CFire_Keeper& rhs)
	: CGameObject(rhs)
	, m_tParams(rhs.m_tParams)
{
}

HRESULT CFire_Keeper::Initialize_Prototype()
{
	m_tParams.fMaxHp = { 590.f };
	m_tParams.fCurrentHp = { 590.f };
	m_tParams.fRigidity = { 0.0f };
	m_tParams.iSouls = { 0 };

	return S_OK;
}

HRESULT CFire_Keeper::Initialize(void* pArg)
{
	m_strObjName = TEXT("Fire_Keeper");

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-5.7f, -11.f, 13.f, 1.f));

	return S_OK;
}

void CFire_Keeper::Tick(_float fTimeDelta)
{

	CModel* pModel = (CModel*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_BackGround"), TEXT("Com_Model"), 1);
	CTransform* pTerrainTransform = (CTransform*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_BackGround"), TEXT("Com_Transform"), 1);

	_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	vWorldPos = XMVectorSetY(vWorldPos, pModel->Compute_Height(vWorldPos, pTerrainTransform));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWorldPos);

	CTransform* pPlayer_Transform = (CTransform*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform"));

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
			if (m_pInteraction_Box == nullptr)
			{
			m_pInteraction_Box = CInteraction::Create(m_pDevice, m_pContext, &strOK);
			}
			m_eDialogueState = STATE_DORMANT;
			m_bIsApproched = false;
		}

	}
	else
	{
		m_bIsApproched = true;
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
		m_pModelCom->Set_Animation(0,true);
		m_pModelCom->Play_Animation(true, fTimeDelta);
	//	}
	//}
	if (m_bIsApproched == false && m_eDialogueState == STATE_DORMANT)
	{
		if (m_pGameInstance->Key_Down('E'))
		{
		
			CUI_NPC_Base::NPC_TYPE eType = CUI_NPC_Base::NPC_FIREKEEPER;
		m_pNPCBase = CUI_NPC_Base::Create(m_pDevice, m_pContext,&eType);
		m_eDialogueState = STATE_ADDRESSED;
		}
		else
		{
		m_pInteraction_Box->Tick(fTimeDelta);
		}
	}
	if (m_bIsApproched == false && m_eDialogueState == STATE_ADDRESSED)
	{
		m_iSelectionIdx = m_pNPCBase->Get_CurrentMenuIdx();
		m_pNPCBase->Tick(fTimeDelta);

		if (m_iSelectionIdx == 0)
		{
			if (m_pGameInstance->Key_Down('E'))
			{

				m_eDialogueState = STATE_TALKING;
			}

		}

		if (m_pGameInstance->Key_Down('Q'))
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_NPC_DIALOGUE);
			CSound_Manager::Get_Instance()->PlaySound(L"FireKeeper_08.mp3", CSound_Manager::SOUND_NPC_DIALOGUE, 1.f);
			m_eDialogueState = STATE_DORMANT;
			m_bIsApproched = true;
		}

	}
	if (m_bIsApproched == false && m_eDialogueState == STATE_TALKING)
	{
		if (m_bIsTalking == false)
		{
				m_pDialogueBG = CUI_Dialogue_BG::Create(m_pDevice, m_pContext);
				m_pScript = CUI_Script_Firekeeper::Create(m_pDevice, m_pContext);
				m_bIsTalking = true;
				m_iDialogueIdx = 0;
				//m_eDialogueState = STATE_ADDRESSED;
		}
		else
		{
			if (m_pGameInstance->Key_Down('E'))
			{
				if (m_iDialogueIdx < 4)
				{
					m_iDialogueIdx++;
					m_bIsSpeaking = false;
					m_pScript->Set_DialougeIdx(m_iDialogueIdx);
				}
				if (m_iDialogueIdx >= 4)
				{
					m_iDialogueIdx = 0;
					m_eDialogueState = STATE_ADDRESSED;
					m_pDialogueBG->Free();
					m_pScript->Free();
					m_bIsTalking = false;
				}

			}
		}

	}

}

void CFire_Keeper::LateTick(_float fTimeDelta)
{
	if (m_bIsSpeaking == false)
	{

	if (m_iDialogueIdx == 0 && m_eDialogueState == STATE_TALKING)
	{
		m_bIsSpeaking = true;
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_NPC_DIALOGUE);
		CSound_Manager::Get_Instance()->PlaySound(L"FireKeeper_17.mp3", CSound_Manager::SOUND_NPC_DIALOGUE, 1.f);
	}
	if (m_iDialogueIdx == 1 && m_eDialogueState == STATE_TALKING)
	{
		m_bIsSpeaking = true;
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_NPC_DIALOGUE);
		CSound_Manager::Get_Instance()->PlaySound(L"FireKeeper_18.mp3", CSound_Manager::SOUND_NPC_DIALOGUE, 1.f);
	}
	if (m_iDialogueIdx == 2 && m_eDialogueState == STATE_TALKING)
	{
		m_bIsSpeaking = true;
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_NPC_DIALOGUE);
		CSound_Manager::Get_Instance()->PlaySound(L"FireKeeper_19.mp3", CSound_Manager::SOUND_NPC_DIALOGUE, 1.f);
	}
	if (m_iDialogueIdx == 3 && m_eDialogueState == STATE_TALKING)
	{
		m_bIsSpeaking = true;
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_NPC_DIALOGUE);
		CSound_Manager::Get_Instance()->PlaySound(L"FireKeeper_20.mp3", CSound_Manager::SOUND_NPC_DIALOGUE, 1.f);
	}

	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);


	if (m_bIsApproched == false && m_eDialogueState == STATE_DORMANT)
	{
		m_pInteraction_Box->LateTick(fTimeDelta);

	}
	if (m_bIsApproched == false && m_eDialogueState == STATE_ADDRESSED)
	{
		m_pNPCBase->LateTick(fTimeDelta);
	}
	if (m_bIsApproched == false && m_eDialogueState == STATE_TALKING)
	{
		if (m_pDialogueBG != nullptr)
		{
		m_pDialogueBG->LateTick(fTimeDelta);
		m_pScript->LateTick(fTimeDelta);
		}

	}

}

HRESULT CFire_Keeper::Render()
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


		if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
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

HRESULT CFire_Keeper::Add_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Model_Fire_Keeper"),
		TEXT("Com_Model_Idle"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFire_Keeper::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CFire_Keeper* CFire_Keeper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CFire_Keeper* pInstance = new CFire_Keeper(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFire_Keeper");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFire_Keeper::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CFire_Keeper* pInstance = new CFire_Keeper(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFire_Keeper");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFire_Keeper::Free()
{
	__super::Free();

		
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNPCBase);
	Safe_Release(m_pInteraction_Box);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
