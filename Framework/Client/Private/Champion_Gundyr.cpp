#include "stdafx.h"
#include "Champion_Gundyr.h"
#include "Player.h"
#include "Sound_Manager.h"

#include "GameInstance.h"

CGundyr::CGundyr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CGundyr::CGundyr(const CGundyr& rhs)
	: CGameObject(rhs)
	, m_tParams(rhs.m_tParams)
{
}

HRESULT CGundyr::Initialize_Prototype()
{
	m_tParams.fMaxHp = { 2000.f};
	m_tParams.fCurrentHp = { 2000.f };
	m_tParams.fRigidity = { 0.0f };
	m_tParams.iSouls = { 15000 };

	return S_OK;
}

HRESULT CGundyr::Initialize(void* pArg)
{
	m_strObjName = TEXT("Gundyr");

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(7.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(_float3{ 0.02f,0.02f,0.02f });

	m_vInitialPos = XMVectorSet(296.f, 97.f, -582.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vInitialPos);
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90));

	m_fBehaviorTimer = 1.5f;

	m_pWeaponColliderCom->Ready_Attack_Behavior(&m_tMonster_Attack);

	m_pPlayerTransform = (CTransform*)m_pGameInstance->Get_Component(LEVEL_GUNDYR, TEXT("Layer_Player"), TEXT("Com_Transform"));

	wstring strBossName = TEXT("영웅, 군다");

	m_pBoss_Hp_Base = CBoss_Bar_Base::Create(m_pDevice, m_pContext, &strBossName);

	m_pBoss_Hp_Bar = CBoss_Hp_Bar::Create(m_pDevice, m_pContext);

	m_RandomNumber = mt19937_64(m_RandomDevice());

	return S_OK;
}

void CGundyr::Tick(_float fTimeDelta)
{

	CModel* pModel = (CModel*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_BackGround"), TEXT("Com_Model"), 1);
	CTransform* pTerrainTransform = (CTransform*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_BackGround"), TEXT("Com_Transform"), 1);

	_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vWorldPos = XMVectorSetY(vWorldPos, pModel->Compute_Height(vWorldPos, pTerrainTransform,false));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWorldPos);

	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = XMVectorSetY(vPlayerPos - vPosition, 0.f);
	_vector vFlatLook = XMVectorSetY((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), 0.f);
		
	m_fIgnoreTimer -= fTimeDelta;
	if (m_fIgnoreTimer < 0.f)
	{
		m_iIgnoreID = -1;
	}

	if (m_tParams.fCurrentHp < 0.f)
	{
		//Death Motion
		if (m_bInvoke_Behavior)
		{
		CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Die.wav", CSound_Manager::SOUND_MONSTER_EFFECT4, 1.f);
		m_bInvoke_Behavior = false;
		}
		m_iNextAnimIndex = 57;
		m_pModelCom->Set_Animation(57, false);
		m_bIsLoop = false;
		if (m_bIsFinished)
		{
			//Dead
		m_bIsAlert = false;
		m_iNextAnimIndex = 58;
		m_bIsLoop = true;
		}

	}
	else
	{
		if (m_tParams.fCurrentHp/ m_tParams.fMaxHp < 0.5f && m_bPhaseII==false)
		{
			CSound_Manager::Get_Instance()->StopAll();
			CSound_Manager::Get_Instance()->PlayBGM(L"BGM_Gunda_1.wav", 0.7f);
			m_iBehaviorIndex = 5;
			m_iBehaviorStage = 0;
			m_bPhaseII = true;
		}

		if ( XMVector3Length(vToPlayer).m128_f32[0] <= 40.f && m_bIsAlert == false)
		{

			m_iBehaviorIndex = 6;
			m_bIsAlert = true;

			CSound_Manager::Get_Instance()->StopAll();
			CSound_Manager::Get_Instance()->PlayBGM(L"BGM_Gunda_0.wav", 0.7f);
		}

		// Is in Battle Against Player
		if (m_bIsAlert == true)
		{

			
			m_fBehaviorTimer -= fTimeDelta;
			if (m_fBehaviorTimer < 0 && m_bMoveFinished)
			{
				//Change Moveset Index when Every 2Seconds But Only if Current moveset is Finished
				Determine_Behavior();
				m_fBehaviorTimer = 2.f;
			}

			Execute_Behavior(fTimeDelta);
		}

	}

		m_pWeaponColliderCom->Update(XMLoadFloat4x4(m_pModelCom->Get_BoneCombinedMatrix("pole_Bone_01"))* XMLoadFloat4x4(&m_pModelCom->Get_PivotMatrix()) * m_pTransformCom->Get_WorldMatrix());

		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	//if (m_pColliderCom->Intersect((CCollider*)m_pGameInstance->Get_Component(LEVEL_GUNDYR, TEXT("Layer_Player"), TEXT("Com_Collider_SPHERE"), 0, CPlayer::PART_RHAND)) == true)
	//{
	//	((CCollider*)m_pGameInstance->Get_Component(LEVEL_GUNDYR, TEXT("Layer_Player"), TEXT("Com_Collider_SPHERE"), 0, CPlayer::PART_RHAND))->Inactivate_Collider();

	//}	

	//if(m_pGameInstance->Get_DIKeyState(DIK_K))
	//{
	//	m_tParams.fCurrentHp -= 10.f;
	//	m_pBoss_Hp_Base->Update(10.f);
	//}

		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = m_iNextAnimIndex;
		m_pModelCom->Set_Animation(m_iCurrentAnimIndex, m_bIsLoop);

		CCollider* pPlayerHand_Collider = ((CCollider*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Collider_SPHERE"), 0, CPlayer::PART_RHAND));

		if (m_pColliderCom->Intersect(pPlayerHand_Collider) == true)
		{

			CCollider::ATTACK_DESC pPlayerAttack = pPlayerHand_Collider->Get_Attack_Desc();

			if (pPlayerHand_Collider->Get_Collider_State() == CCollider::STATE_NORMAL
				&& m_iIgnoreID != pPlayerAttack.iBehaviorID)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT3);
				CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Hit.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 1.f);

				m_tParams.fCurrentHp -= pPlayerAttack.fDamage;
				m_pBoss_Hp_Base->Update(pPlayerAttack.fDamage);
				m_iIgnoreID = pPlayerAttack.iBehaviorID;
				m_fIgnoreTimer = 0.6f;;
				pPlayerHand_Collider->Inactivate_Collider();

			}

		}


	if (m_bIsAlert == true)
	{
		m_pBoss_Hp_Base->Tick(fTimeDelta);
		m_pBoss_Hp_Bar->Update(m_tParams.fCurrentHp / m_tParams.fMaxHp);
	}
}

void CGundyr::LateTick(_float fTimeDelta)
{

	Execute_Animation(m_iCurrentAnimIndex, fTimeDelta);
	m_bIsFinished = m_pModelCom->Play_Animation(m_bIsLoop, fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	if (m_bIsAlert == true)
	{
		m_pBoss_Hp_Base->LateTick(fTimeDelta);
		m_pBoss_Hp_Bar->LateTick(fTimeDelta);
	}
}

HRESULT CGundyr::Render()
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

#ifdef _DEBUG
	m_pColliderCom->Render();
	m_pWeaponColliderCom->Render();
#endif


	return S_OK;
}

HRESULT CGundyr::Add_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_GUNDYR, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Components(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_Champion_Gundyr"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		BoundingDesc = {};
	BoundingDesc.vExtents = _float3(2.1f, 4.f, 2.1f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	BoundingDesc.vRadians = _float3(0.f, XMConvertToRadians(0.f), 0.f);


	if (FAILED(__super::Add_Components(LEVEL_GUNDYR, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), (CComponent**)&m_pColliderCom, &BoundingDesc)))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	CBounding_SPHERE::BOUNDING_SPHERE_DESC		WeaponBoundingDesc = {};
	WeaponBoundingDesc.fRadius = 80.f;
	WeaponBoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 110.f);

	if (FAILED(__super::Add_Components(LEVEL_GUNDYR, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider_SPHERE"), (CComponent**)&m_pWeaponColliderCom, &WeaponBoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGundyr::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CGundyr::Determine_Behavior()
{
	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = XMVectorSetY(vPlayerPos - vPosition, 0.f);
	_vector vFlatLook = XMVectorSetY((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), 0.f);

	//Behaviors 


	// if Moveset has been selected and Has NOT finished
	if (m_iBehaviorIndex != 1)
	{
		return;
	}
	else
	{
		//Determine Behavior

	//Player is Not at Front
	//if (XMVector3Dot(XMVector3Normalize(vFlatLook), XMVector3Normalize(vToPlayer)).m128_f32[0] < .95f)
	//{
	//	if (XMVector3Dot(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)), XMVector3Normalize(vToPlayer)).m128_f32[0] > 0)
	//	{
	//	m_iBehaviorIndex = 4;

	//	}
	//	else
	//	{
	//	m_iBehaviorIndex = 5;

	//	}


	//	return;
	//}

	//Monster Has Deviated from Patrol Range
	//if (XMVector3Length(m_vInitialPos - vPosition).m128_f32[0] >= 25.f)
	//{
	//	// Return to Original Position
	//	m_iBehaviorIndex = 6;
	//	return;
	//}

	//Player is Within Attack Range
	if (XMVector3Length(vToPlayer).m128_f32[0] <= 3.f)
	{
		//Attack Player
		m_iBehaviorIndex = 1;
		return;

	}

	////Player is Within Battle Range
	//if (XMVector3Length(vToPlayer).m128_f32[0] <= 5.f)
	//{
	//	//Walk to Player & Attack 
	//	m_iBehaviorIndex = 2;
	//	return;


	//}

	////Player is Far From Monster
	//if (XMVector3Length(vToPlayer).m128_f32[0] <= 10.f)
	//{
	//		//Run Towards Player & Attack 
	//		m_iBehaviorIndex = 3;
	//		return;

	//}
	}

}

void CGundyr::Execute_Behavior(_float fTimeDelta)
{
	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = vPlayerPos - vPosition;
	_vector vDirToPlayer = XMVectorSetY(vToPlayer, 0.f);
	_float	vDistance = XMVector3Length(vToPlayer).m128_f32[0];
	_vector vFlatLook = XMVectorSetY((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), 0.f);

	m_fAtk_Interval -= fTimeDelta;

	m_bMoveFinished = false;

	if (m_iBehaviorIndex == 5 && m_iBehaviorStage == 0)
	{
		//Init Phase II
		m_iNextAnimIndex = 1;
		m_bIsLoop = false;

		if (m_bIsFinished == true)
		{
			m_iNextAnimIndex = 22;
			m_bIsLoop = false;
			m_bMoveFinished = false;

			m_iBehaviorIndex = 7;
			m_iBehaviorStage = 0;
		}
	}

	if (m_iBehaviorIndex == 6 && m_iBehaviorStage == 0)
	{
		//Init Phase I
		m_iNextAnimIndex = 64;
		m_bIsLoop = false;
		m_iBehaviorStage++;

	}
	else if (m_iBehaviorIndex == 6 && m_iBehaviorStage == 1)
	{
		if (m_bIsFinished == true)
		{
			m_iBehaviorIndex = 7;
			m_iBehaviorStage = 0;
			m_bMoveFinished = true;
		}
	}

	if (m_iBehaviorIndex == 7 && m_iBehaviorStage == 0)
	{
		//If Player Is Not Within Attack Angle
		if (XMVector3Dot(XMVector3Normalize(vFlatLook), XMVector3Normalize(vDirToPlayer)).m128_f32[0] < .5f)
		{
			// Determine Turn Direction
			if (XMVector3Dot(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)), XMVector3Normalize(vDirToPlayer)).m128_f32[0] > 0)
			{
				//Turn Left
				m_iNextAnimIndex = 27;
				m_bIsLoop = true;

			}
			else
			{
				//Turn Right
				m_iNextAnimIndex = 28;
				m_bIsLoop = true;
			}

		}
		else
		{
			//if Player is within Attack Angle

			//if Boss Has Attacked Recently
			if (m_fAtk_Interval>0.f)
			{
				//Player is at Close Range
				if (vDistance <= 10.f)
				{
					//Move Back
				m_iNextAnimIndex = 3;
				m_bIsLoop = true;
				}

				//Move SideWays, Yhrom = No SideSteps
				if (vDistance > 10.f)
				{
					m_iNextAnimIndex = 4;
					m_bIsLoop = true;
				}
			}
			else
			{

			//Player is Far away
			if (vDistance >= 30.f)
			{
				if ((m_tParams.fCurrentHp / m_tParams.fMaxHp) > 0.5f)
				{
					switch (m_iRandom % 2)
					{
					case 0:
						m_iNextAnimIndex = 9;
						m_bIsLoop = false;
						m_bIsFinished = false;
						break;
					case 1:
						m_bRun = true;
						break;
					default:
						break;
					}
				}
				else
				{
					switch (m_iRandom % 3)
					{
					case 0:
						m_iNextAnimIndex = 9;
						m_bIsLoop = false;
						break;
					case 1:
						m_bRun = true;
						break;
					case 2:
						m_iNextAnimIndex = 1;
						m_bIsLoop = false;
						m_bIsFinished = false;
						break;
					default:
						break;
					}
				}
			}

				if (m_iNextAnimIndex == 9)
				{
					if (m_bIsFinished == true)
					{

						m_iBehaviorStage++;
					}
				}
				else
				{

					if (m_bRun == true)
					{
						m_iNextAnimIndex = 6;
						m_bIsLoop = true;
						//Run Towards Player
						if (vDistance <= 10.f && m_bRun == true)
						{
							//Player is Within Running Attack Range
							m_iBehaviorStage++;
							m_bRun = false;
						}
					}
					else
					{
						//Walk Towards Player
						m_iNextAnimIndex = 2;
						m_bIsLoop = true;
						if (vDistance <= 10.f && m_bRun == false)
						{
							//Player is Within Attack Range
							m_iBehaviorStage++;
							m_bRun = false;
						}
					}
				}
			}
		}
	}
	else if (m_iBehaviorIndex == 7 && m_iBehaviorStage == 1)
	{

		//Player is NOT Within Attack Angle
		if (XMVector3Dot(XMVector3Normalize(vFlatLook), XMVector3Normalize(vDirToPlayer)).m128_f32[0] < .95f)
		{
			//Determine Direction
			if (XMVector3Dot(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)), XMVector3Normalize(vDirToPlayer)).m128_f32[0] > 0)
			{
				//Turn Left
				m_iNextAnimIndex = 27;
				m_bIsLoop = true;

			}
			else
			{
				//Turn Right
				m_iNextAnimIndex = 28;
				m_bIsLoop = true;
			}
		}
		else
		{
			//Monster is Running
			if (m_iPrevAnimIndex == 6 || m_iCurrentAnimIndex == 6)
			{
				//invoke Running Attack
				m_iNextAnimIndex = 7;
				m_bIsLoop = false;
				m_iBehaviorStage++;
			}
			else
			{

				switch (m_iRandom % 5)
				{
				case 0:
					m_iNextAnimIndex = 7;
					break;
				case 1:
					m_iNextAnimIndex = 11;
					break;
				case 2:
					m_iNextAnimIndex = 14;
					break;
				case 3:
					m_iNextAnimIndex = 19;
					break;
				case 4:
					m_iNextAnimIndex = 23;
					break;
				default:
					break;
				}
				m_bIsLoop = false;
				m_iBehaviorStage++;

			}
		}

	}
	else if (m_iBehaviorIndex == 7 && m_iBehaviorStage == 2)
	{

		if (m_bIsFinished == true)
		{
			m_bMoveFinished = true;
			m_iBehaviorStage = 0;
			m_iAtkIndex++;

			m_fAtk_Interval = 1.0f;


			std::uniform_int_distribution<> iRand(1, 100);
			m_iRandom = iRand(m_RandomDevice);

			if (m_iAtkIndex > 5)
				m_iAtkIndex = 0;
		}
	}

}

void CGundyr::Execute_Animation(_uint iCurrentAnimIndex, _float fTimeDelta)
{

	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = vPlayerPos - vPosition;
	_float	vDistance = XMVector3Length(vToPlayer).m128_f32[0];
	_vector vFlatLook = XMVectorSetY((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), 0.f);


	if (iCurrentAnimIndex != m_iPrevAnimIndex)
	{
		m_fCurrentAnimPos = 0.f;
	}
	m_fCurrentAnimPos += fTimeDelta;

	switch (iCurrentAnimIndex)
	{
	case 0:
		m_bInvoke_Behavior = true;
		break;
	case 1:
		if (m_fCurrentAnimPos <= 0.3f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.f && m_fCurrentAnimPos<1.2f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_WarCry.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.7f);
		}
		if (m_fCurrentAnimPos >= 3.f)
		{
			m_iNextAnimIndex = 22;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 2:
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 1.5f);
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_bInvoke_Behavior = true;
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.35f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk0.wav", CSound_Manager::SOUND_MONSTER_EFFECT4, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 0.95f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk1.wav", CSound_Manager::SOUND_MONSTER_EFFECT5, 0.4f);
		}
		if (m_fCurrentAnimPos >= 1.5f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT4);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT5);
			m_fCurrentAnimPos = 0.f;
		}
		break;
	case 3:
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 1.5f);
		m_pTransformCom->Go_Backward(fTimeDelta);
		m_bInvoke_Behavior = true;
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.35f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk0.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 0.95f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk1.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.4f);
		}
		if (m_fCurrentAnimPos >= 1.5f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT2);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT3);
			m_fCurrentAnimPos = 0.f;
		}
		break;
	case 4:
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 1.5f);
		m_pTransformCom->Go_Left(fTimeDelta);
		m_bInvoke_Behavior = true;
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.35f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk0.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 0.95f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk1.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.4f);
		}
		if (m_fCurrentAnimPos >= 1.5f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT2);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT3);
			m_fCurrentAnimPos = 0.f;
		}
		break;
	case 5:
		m_bInvoke_Behavior = true;
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.35f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk0.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 0.95f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Walk1.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.4f);
		}
		if (m_fCurrentAnimPos >= 1.5f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT2);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT3);
			m_fCurrentAnimPos = 0.f;
		}
		break;
	case 6:
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		m_pTransformCom->Go_Straight(fTimeDelta * 2.8f);
		m_bInvoke_Behavior = true;
		if (m_fCurrentAnimPos >= 0.2f && m_fCurrentAnimPos < 0.25f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Run0.wav", CSound_Manager::SOUND_MONSTER_EFFECT6, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.7f && m_fCurrentAnimPos < 0.75f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Run1.wav", CSound_Manager::SOUND_MONSTER_EFFECT7, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.9f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT6);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT7);
			m_fCurrentAnimPos = 0.f;
		}
		break;
	case 7:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Go_Straight(fTimeDelta * 1.5f);
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.48f && m_fCurrentAnimPos < 0.52f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step0.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
		}
		if (m_fCurrentAnimPos >= 0.69f && m_fCurrentAnimPos < 0.71f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_RF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step1.wav", CSound_Manager::SOUND_MONSTER_RF, .5f);
		}
		if (m_fCurrentAnimPos >= 1.f && m_fCurrentAnimPos < 1.15f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step2.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.15f && m_fCurrentAnimPos < 1.45f)
		{
			m_bInvoke_Behavior = true;
		}
		if (m_fCurrentAnimPos >= 1.5f && m_fCurrentAnimPos < 1.7f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.7f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 2.f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 8;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 8:

		if (m_fCurrentAnimPos >= 0.8f && m_fCurrentAnimPos < 0.85f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_RF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step1.wav", CSound_Manager::SOUND_MONSTER_RF, .5f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 0.85f && m_fCurrentAnimPos < 1.7f)
		{
			m_bInvoke_Behavior = true;
		}

		if (m_fCurrentAnimPos >= 1.7f && m_fCurrentAnimPos < 1.9f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.9f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 9:
		//Jump Attack
		if (m_fCurrentAnimPos <= 0.1f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.f && m_fCurrentAnimPos < 2.1f)
		{
			m_pTransformCom->Go_Straight(fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.1f && m_fCurrentAnimPos < 1.15f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step2.wav", CSound_Manager::SOUND_MONSTER_EFFECT, .8f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.15f && m_fCurrentAnimPos < 1.9f)
		{
			m_bInvoke_Behavior = true;
		}
		if (m_fCurrentAnimPos >= 2.0f && m_fCurrentAnimPos < 2.1f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing4.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.7f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.1f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		break;
	case 10:

		break;
	case 11:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}

		if (m_fCurrentAnimPos >= 0.7f && m_fCurrentAnimPos < 0.75f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step0.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 0.75f && m_fCurrentAnimPos < 1.9f)
		{
			m_bInvoke_Behavior = true;
		}

		if (m_fCurrentAnimPos >= 1.9f && m_fCurrentAnimPos < 2.1f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.1f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 2.1f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 12;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 12:
		if (m_fCurrentAnimPos <= 0.2f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.6f && m_fCurrentAnimPos < 0.8f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_BodySlam.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.7f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 0.8f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 0.9f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 13;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 13:
		if (m_fCurrentAnimPos >= 1.3f && m_fCurrentAnimPos < 1.5f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.7f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.5f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 14:
		if (m_fCurrentAnimPos <= 0.3f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.35f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step0.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
		}

		if (m_fCurrentAnimPos >= 0.8f && m_fCurrentAnimPos < 0.85f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_RF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step1.wav", CSound_Manager::SOUND_MONSTER_RF, .5f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 0.85f && m_fCurrentAnimPos < 1.8f)
		{
			m_bInvoke_Behavior = true;
		}


		if (m_fCurrentAnimPos >= 1.8f && m_fCurrentAnimPos < 2.2f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.2f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 2.2f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 15;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 15:

		if (m_fCurrentAnimPos >= 1.4f && m_fCurrentAnimPos < 1.45f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step0.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.45f && m_fCurrentAnimPos < 2.1f)
		{
			m_bInvoke_Behavior = true;
		}

		if (m_fCurrentAnimPos >= 2.1f && m_fCurrentAnimPos < 2.3f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.3f && m_fCurrentAnimPos< 2.55f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		if (m_fCurrentAnimPos >= 2.6f && m_fCurrentAnimPos < 2.8f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.8f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 16:
		break;
	case 17:
		break;
	case 18:
		break;
	case 19:
		if (m_fCurrentAnimPos <= 0.3f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 1.0f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.0f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		if (m_fCurrentAnimPos >= 1.0f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 20;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 20:
		if (m_fCurrentAnimPos <= 0.2f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 1.0f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.0f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		if (m_fCurrentAnimPos >= 1.0f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 21;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 21:

		if (m_fCurrentAnimPos >= 1.2f && m_fCurrentAnimPos < 1.25f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step0.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.25f && m_fCurrentAnimPos < 1.3f)
		{
			m_bInvoke_Behavior = true;
		}

		if (m_fCurrentAnimPos >= 1.3f && m_fCurrentAnimPos < 1.5f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.5f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 22:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.6f && m_fCurrentAnimPos < 0.65f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step0.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
		}
		if (m_fCurrentAnimPos >= 0.8f && m_fCurrentAnimPos < 0.85f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_RF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step1.wav", CSound_Manager::SOUND_MONSTER_RF, .5f);
		}
		if (m_fCurrentAnimPos >= 1.1f && m_fCurrentAnimPos < 1.15f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_LF);
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Step2.wav", CSound_Manager::SOUND_MONSTER_LF, .5f);
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.15f && m_fCurrentAnimPos < 1.5f)
		{
			m_bInvoke_Behavior = true;
		}

		if (m_fCurrentAnimPos >= 1.5f && m_fCurrentAnimPos < 1.7f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.7f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 23:
		if (m_fCurrentAnimPos <= 0.3f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.7f && m_fCurrentAnimPos < 1.0f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_Spear.wav", CSound_Manager::SOUND_MONSTER_EFFECT, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.0f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		if (m_fCurrentAnimPos >= 1.2f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 25;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 24:
		break;
	case 25:
		if (m_fCurrentAnimPos <= 0.2f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 1.0f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Gundyr_BodySlam.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.5f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.0f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 26:
		break;
	case 27:
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 2.f);
		break;
	case 28:
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * 2.f);
		break;
	case 29:

		break;
	case 30:
		break;
	case 31:
		break;
	case 32:

		break;
	case 33:

		break;
	case 34:

		break;
	case 35:
		break;
	case 36:
		break;
	case 37:
		break;
	case 38:
		break;
	case 39:
		break;
	case 40:
		break;	
	case 41:
		break;
	case 42:
		break;
	case 43:
		break;
	case 44:
		break;
	case 45:
		break;
	case 46:
		break;
	case 47:
		break;
	case 48:
		break;
	case 49:
		break;
	case 50:
		break;
	case 51:
		break;
	case 52:
		break;
	case 53:
		break;
	case 54:
		break;
	case 55:
		break;
	case 56:
		break;
	case 57:
		break;
	case 58:
		if (m_fCurrentAnimPos >= 3.f)
			m_bDestroy = true;
		break;
	case 59:
		break;
	case 60:
		break;
	case 61:
		break;
	case 62:
		break;
	case 63:
		break;
	case 64:
		break;
	default:
		break;
	}
}

CGundyr* CGundyr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CGundyr* pInstance = new CGundyr(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGundyr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGundyr::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CGundyr* pInstance = new CGundyr(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGundyr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGundyr::Free()
{
	__super::Free();
	
	if (m_pBoss_Hp_Base != nullptr)
	{
		Safe_Release(m_pBoss_Hp_Base);
		m_pBoss_Hp_Base = nullptr;
	}
	if (m_pBoss_Hp_Bar != nullptr)
	{
		Safe_Release(m_pBoss_Hp_Bar);
		m_pBoss_Hp_Bar = nullptr;
	}

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pWeaponColliderCom);

}
