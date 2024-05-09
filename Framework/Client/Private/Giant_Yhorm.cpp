#include "stdafx.h"
#include "Giant_Yhorm.h"
#include "Player.h"
#include "Sound_Manager.h"

#include "GameInstance.h"

CYhorm::CYhorm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CYhorm::CYhorm(const CYhorm& rhs)
	: CGameObject(rhs)
	, m_tParams(rhs.m_tParams)
{
}

HRESULT CYhorm::Initialize_Prototype()
{
	m_tParams.fMaxHp = { 5000.f};
	m_tParams.fCurrentHp = { 5000.f };
	m_tParams.fRigidity = { 0.0f };
	m_tParams.iSouls = { 20000 };

	return S_OK;
}

HRESULT CYhorm::Initialize(void* pArg)
{
	m_strObjName = TEXT("Yhorm");

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(7.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(_float3{ 0.02f,0.02f,0.02f });

	m_vInitialPos = XMVectorSet(-220.f, 0.f, -150.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vInitialPos);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45));

	m_fBehaviorTimer = 1.f;

	m_pWeaponColliderCom->Ready_Attack_Behavior(&m_tMonster_Attack);

	m_pPlayerTransform = (CTransform*)m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	wstring strBossName = TEXT("거인 욤");

	m_pBoss_Hp_Base = CBoss_Bar_Base::Create(m_pDevice, m_pContext, &strBossName);

	m_pBoss_Hp_Bar = CBoss_Hp_Bar::Create(m_pDevice, m_pContext);

	return S_OK;
}

void CYhorm::Tick(_float fTimeDelta)
{

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
		CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Die.wav", CSound_Manager::SOUND_MONSTER_EFFECT6, 1.f);
		m_iNextAnimIndex = 37;
		m_bIsLoop = false;
		if (m_bIsFinished)
		{
		m_bIsAlert = false;
		m_iNextAnimIndex = 38;
		m_bIsLoop = true;
		}

	}
	else
	{


		if ( XMVector3Length(vToPlayer).m128_f32[0] <= 50.f && m_bIsAlert == false)
		{

			m_iBehaviorIndex = 6;
			m_bIsAlert = true;
			CSound_Manager::Get_Instance()->StopAll();
			CSound_Manager::Get_Instance()->PlayBGM(L"BGM_Begin_Arena.mp3", 0.7f);

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

		m_pWeaponColliderCom->Update(XMLoadFloat4x4(m_pModelCom->Get_BoneCombinedMatrix("R_Weapon001"))* XMLoadFloat4x4(&m_pModelCom->Get_PivotMatrix()) * m_pTransformCom->Get_WorldMatrix());

		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

		CCollider* pPlayerHand_Collider = ((CCollider*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Collider_SPHERE"), 0, CPlayer::PART_RHAND));

		if (m_pColliderCom->Intersect(pPlayerHand_Collider) == true)
		{

			CCollider::ATTACK_DESC pPlayerAttack = pPlayerHand_Collider->Get_Attack_Desc();

			if (pPlayerHand_Collider->Get_Collider_State() == CCollider::STATE_NORMAL
				&& m_iIgnoreID != pPlayerAttack.iBehaviorID)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT3);
				CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Hit.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 1.f);

				m_tParams.fCurrentHp -= pPlayerAttack.fDamage;
				m_pBoss_Hp_Base->Update(pPlayerAttack.fDamage);
				m_iIgnoreID = pPlayerAttack.iBehaviorID;
				m_fIgnoreTimer = 0.7f;
				pPlayerHand_Collider->Inactivate_Collider();

			}

		}

		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = m_iNextAnimIndex;
		m_pModelCom->Set_Animation(m_iCurrentAnimIndex, m_bIsLoop);


	if (m_bIsAlert == true)
	{
		m_pBoss_Hp_Base->Tick(fTimeDelta);
		m_pBoss_Hp_Bar->Update(m_tParams.fCurrentHp / m_tParams.fMaxHp);
	}
}

void CYhorm::LateTick(_float fTimeDelta)
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

HRESULT CYhorm::Render()
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

HRESULT CYhorm::Add_Component()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Yhrom"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		BoundingDesc = {};
	BoundingDesc.vExtents = _float3(5.f, 5.f, 3.5f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	BoundingDesc.vRadians = _float3(0.f, XMConvertToRadians(0.f), 0.f);


	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), (CComponent**)&m_pColliderCom, &BoundingDesc)))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	CBounding_SPHERE::BOUNDING_SPHERE_DESC		WeaponBoundingDesc = {};
	WeaponBoundingDesc.fRadius = 170.f;
	//WeaponBoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	WeaponBoundingDesc.vCenter = _float3(0.f, 0.f, 250.f);

	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider_SPHERE"), (CComponent**)&m_pWeaponColliderCom, &WeaponBoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CYhorm::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CYhorm::Determine_Behavior()
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

void CYhorm::Execute_Behavior(_float fTimeDelta)
{
	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = vPlayerPos - vPosition;
	_vector vDirToPlayer = XMVectorSetY(vToPlayer, 0.f);
	_float	vDistance = XMVector3Length(vToPlayer).m128_f32[0];
	_vector vFlatLook = XMVectorSetY((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), 0.f);

	m_fAtk_Interval -= fTimeDelta;

	m_bMoveFinished = false;

	if (m_iBehaviorIndex == 6 && m_iBehaviorStage == 0)
	{
		m_iNextAnimIndex = 39;
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

				m_iNextAnimIndex = 34;	
				m_bIsLoop = true;

			}
			else
			{
				m_iNextAnimIndex = 33;
				m_bIsLoop = true;
			}

		}
		else
		{
			//if Player is within Attack Angle
			//if Boss Has Attacked Recently
			if (m_fAtk_Interval>0.f)
			{
				m_iNextAnimIndex = 1;
				m_bIsLoop = true;
				//Player is at Close Range
				if (vDistance <= 3.f)
				{
					//Move Back
				m_iNextAnimIndex = 7;
				m_bIsLoop = true;
				}

				//Can Move SideWays, Yhrom = No SideSteps

			}
			else
			{

			//Player is Far away
			if (vDistance >= 30.f)
			{
				m_iNextAnimIndex = 31;
				m_bRun = true;
			}

			if (m_bRun == true)
			{
				//Run Towards Player
				m_iNextAnimIndex = 9;
				m_bIsLoop = true;
			}
			else
			{
				//Walk Towards Player
				m_iNextAnimIndex = 6;
				m_bIsLoop = true;
			}
		if (vDistance <= 15.f && m_bRun==true)
		{
			//Player is Within Running Attack Range
				m_iBehaviorStage = 1;
				m_bRun = false;
		}

		if (vDistance <= 10.f && m_bRun == false)
		{
			//Player is Within Attack Range
			m_iBehaviorStage = 1;
			m_bRun = false;
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
				//Turn Right
				m_iNextAnimIndex = 34;
				m_bIsLoop = true;

			}
			else
			{
				//Turn Left
				m_iNextAnimIndex = 33;
				m_bIsLoop = true;
			}
		}
		else
		{
			//Monster is Running
			if (m_iPrevAnimIndex == 9 || m_iCurrentAnimIndex == 9)
			{
				//invoke Running Attack
				m_iNextAnimIndex = 27;
				m_bIsLoop = false;
				m_iBehaviorStage++;
			}
			else
			{

				switch (m_iAtkIndex)
				{
				case 0:
					break;
				case 1:
					m_iNextAnimIndex = 10;
					break;
				case 2:
					m_iNextAnimIndex = 14;
					break;
				case 3:
					m_iNextAnimIndex = 21;
					break;
				case 4:
					m_iNextAnimIndex = 26;
					break;
				case 5:
					m_iNextAnimIndex = 28;
					break;
				case 6:
					m_iNextAnimIndex = 23;
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

			m_fAtk_Interval = 1.5f;

			if (m_iAtkIndex > 7)
				m_iAtkIndex = 0;
		}
	}

}

void CYhorm::Execute_Animation(_uint iCurrentAnimIndex, _float fTimeDelta)
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
	case 1:
		m_bInvoke_Behavior = true;
		break;
	case 2:
		//Blank
		break;
	case 3:
		//Blank
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		m_bInvoke_Behavior = true;
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 1.5f);
		m_pTransformCom->Go_Straight(fTimeDelta);
		if (m_fCurrentAnimPos >= 0.45f && m_fCurrentAnimPos< 0.5f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Walk0.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 0.95f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Walk1.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.4f);
		}
		if(m_fCurrentAnimPos >= 1.f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT2);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT3);
			m_fCurrentAnimPos = 0.f;
		}
		break;
	case 7:
		m_bInvoke_Behavior = true;
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 1.5f);
		m_pTransformCom->Go_Backward(fTimeDelta);
		break;
	case 8:
		//Blank
		break;
	case 9:
		//Run Front
		m_bInvoke_Behavior = true;
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.f);
		m_pTransformCom->Go_Straight(fTimeDelta * 3.5f);

		if (m_fCurrentAnimPos >= 0.35f && m_fCurrentAnimPos < 0.4f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Walk0.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.7f && m_fCurrentAnimPos < 0.75f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Walk1.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.75f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT2);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_MONSTER_EFFECT3);
			m_fCurrentAnimPos = 0.f;
		}

		break;
	case 10:

		if (m_fCurrentAnimPos >= 1.6f && m_fCurrentAnimPos < 2.05f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing1.wav", CSound_Manager::SOUND_MONSTER_EFFECT, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_tMonster_Attack.iBehaviorID = 10;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.05f)
		{
			m_pWeaponColliderCom->Inactivate_Collider();
			m_bInvoke_Behavior = true;
		}

		if (m_fCurrentAnimPos >= 2.05f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 11;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 11:
		if (m_fCurrentAnimPos >= 1.9f && m_fCurrentAnimPos < 2.2f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing4.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_tMonster_Attack.iBehaviorID = 10;
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
			m_iNextAnimIndex = 12;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 12:
		if (m_fCurrentAnimPos >= 1.f && m_fCurrentAnimPos < 1.5f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing2.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_tMonster_Attack.iBehaviorID = 10;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.5f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		break;
	case 13:
		if (m_fCurrentAnimPos >= 1.1f && m_fCurrentAnimPos < 2.f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing5.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 14:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);

		}
		if (m_fCurrentAnimPos >= 1.6f && m_fCurrentAnimPos < 1.9f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing5.wav", CSound_Manager::SOUND_MONSTER_EFFECT, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.9f)
		{
			m_pWeaponColliderCom->Inactivate_Collider();
			m_bInvoke_Behavior = true;
		}
		if (m_fCurrentAnimPos >= 1.9f && vDistance <= 20.f)
		{
			m_iNextAnimIndex = 15;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 15:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >=1.f && m_fCurrentAnimPos < 1.2f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing7.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.2f)
		{
			m_pWeaponColliderCom->Inactivate_Collider();
			m_bInvoke_Behavior = true;
		}
		if (m_fCurrentAnimPos >= 1.2f && vDistance <= 20.f)
		{
			m_iNextAnimIndex = 16;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 16:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.5f && m_fCurrentAnimPos < 2.f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing5.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 17:
		if (m_fCurrentAnimPos >= 1.5f && m_fCurrentAnimPos < 2.f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing5.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 18:
		if (m_fCurrentAnimPos >= 0.1f && m_fCurrentAnimPos < 5.f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing0.wav", CSound_Manager::SOUND_MONSTER_EFFECT, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
		}
		if (m_fCurrentAnimPos >= 0.6f)
		{
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 19:
		if (m_fCurrentAnimPos >= 0.1f && m_fCurrentAnimPos < 5.f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing1.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.4f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
		}
		if (m_fCurrentAnimPos >= 0.6f)
		{
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 20:
		break;
	case 21:
		if (m_fCurrentAnimPos >= 2.f && m_fCurrentAnimPos < 2.4f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing0.wav", CSound_Manager::SOUND_MONSTER_EFFECT, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.4f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 2.4f && vDistance <= 10.f)
		{
			m_iNextAnimIndex = 22;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 22:
		if (m_fCurrentAnimPos >= 1.2f && m_fCurrentAnimPos < 1.4f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing1.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.4f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		break;
	case 23:
		if (m_fCurrentAnimPos >= 1.2f && m_fCurrentAnimPos < 1.4f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing2.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.4f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		if (m_fCurrentAnimPos <= 0.1f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.4f && vDistance <= 20.f)
		{
			m_iNextAnimIndex = 24;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 24:
		if (m_fCurrentAnimPos >= 1.f && m_fCurrentAnimPos < 1.4f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing6.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.4f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
		if (m_fCurrentAnimPos <= 0.1f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.4f && vDistance <= 20.f)
		{
			m_iNextAnimIndex = 25;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 25:
		if (m_fCurrentAnimPos >= 1.2f && m_fCurrentAnimPos < 1.5f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing7.wav", CSound_Manager::SOUND_MONSTER_EFFECT4, 0.6f);
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
	case 26:

		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.6f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Roar.wav", CSound_Manager::SOUND_MONSTER_EFFECT6, 1.f);
		}
		if (m_fCurrentAnimPos >= 1.3f && m_fCurrentAnimPos < 1.6f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing7.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.6f && m_fCurrentAnimPos <2.25f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 2.3f && m_fCurrentAnimPos < 2.5f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing2.wav", CSound_Manager::SOUND_MONSTER_EFFECT2, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 2.5f && m_fCurrentAnimPos < 3.95f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 4.f && m_fCurrentAnimPos < 4.3f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing6.wav", CSound_Manager::SOUND_MONSTER_EFFECT3, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 4.3f && m_fCurrentAnimPos < 5.45f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 5.5f && m_fCurrentAnimPos < 6.f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing4.wav", CSound_Manager::SOUND_MONSTER_EFFECT4, 0.6f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 6.f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		break;
	case 27:

		if (m_fCurrentAnimPos >= 0.55f && m_fCurrentAnimPos <0.6f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Walk0.wav", CSound_Manager::SOUND_MONSTER_EFFECT4, 0.4f);

		}
		if (m_fCurrentAnimPos >= 1.3f && m_fCurrentAnimPos < 1.4f)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing4.wav", CSound_Manager::SOUND_MONSTER_EFFECT5, 0.7f);
		}
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Go_Straight(fTimeDelta * 1.5);
		}
		break;
	case 28:
		if (m_fCurrentAnimPos >= 0.9f && m_fCurrentAnimPos < 1.1f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing1.wav", CSound_Manager::SOUND_MONSTER_EFFECT, 0.7f);

			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 1.1f && m_fCurrentAnimPos < 2.6f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}

		if (m_fCurrentAnimPos >= 2.7f && m_fCurrentAnimPos < 3.f && m_bInvoke_Behavior == true)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"Yhorm_Swing4.wav", CSound_Manager::SOUND_MONSTER_EFFECT1, 0.7f);
			m_tMonster_Attack.eAttackType = CCollider::ATK_HEAVY;
			m_tMonster_Attack.fDamage = 100.f;
			m_pWeaponColliderCom->Activate_Collider();
			m_bInvoke_Behavior = false;
		}
		if (m_fCurrentAnimPos >= 3.f)
		{
			m_bInvoke_Behavior = true;
			m_pWeaponColliderCom->Inactivate_Collider();
		}
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
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * 2.f);
		break;
	case 34:
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 2.f);
		break;
	case 35:
	
		break;
	case 36:
		break;
	case 37:
		break;
	case 38:
		if (m_fCurrentAnimPos >= 3.f)
			m_bDestroy = true;
		break;
	case 39:
		break;
	case 40:
		break;
	default:
		break;
	}
}

CYhorm* CYhorm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CYhorm* pInstance = new CYhorm(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CYhorm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CYhorm::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CYhorm* pInstance = new CYhorm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CYhorm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYhorm::Free()
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
