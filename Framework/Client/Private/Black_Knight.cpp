#include "stdafx.h"
#include "Black_Knight.h"
#include "Player.h"

#include "GameInstance.h"

CBlack_Knight::CBlack_Knight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBlack_Knight::CBlack_Knight(const CBlack_Knight& rhs)
	: CGameObject(rhs)
	, m_tParams(rhs.m_tParams)
{
}

HRESULT CBlack_Knight::Initialize_Prototype()
{
	m_tParams.fMaxHp = { 590.f };
	m_tParams.fCurrentHp = { 590.f };
	m_tParams.fRigidity = { 0.0f };
	m_tParams.iSouls = { 0 };

	return S_OK;
}

HRESULT CBlack_Knight::Initialize(void* pArg)
{
	m_strObjName = TEXT("Black_Knight");

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(7.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(_float3{ 0.02f,0.02f,0.02f });

	m_vInitialPos = XMVectorSet(-20.f, 0.f, 40.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vInitialPos);

	m_fBehaviorTimer = 2.f;


	m_pPlayerTransform = (CTransform*)m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	m_pWeaponColliderCom->Ready_Attack_Behavior(&m_tMonster_Attack);

	_vector vPos;
	vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	m_pHp_Bar_Base = m_pHp_Bar_Base->CUI_Monster_Hp_Base::Create(m_pDevice, m_pContext, &vPos);

	return S_OK;
}

void CBlack_Knight::Tick(_float fTimeDelta)
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
		//Death Motion
		m_iNextAnimIndex = 25;
		m_bIsLoop = false;
		if (m_bIsFinished)
		{
			//Dead
			m_bIsAlert = false;
			m_iNextAnimIndex = 26;
			m_bIsLoop = true;
		}

	}
	else
	{
		if (XMVector3Dot(XMVector3Normalize(vToPlayer), XMVector3Normalize(vFlatLook)).m128_f32[0] > 0 &&
			XMVector3Length(vToPlayer).m128_f32[0] <= 20.f && m_bIsAlert == false)
		{
			m_iBehaviorIndex = 7;
			m_bIsAlert = true;
		}

		if (m_bIsAlert == true)
		{

			m_fBehaviorTimer -= fTimeDelta;
			if (m_fBehaviorTimer < 0 && m_bMoveFinished)
			{
				Determine_Behavior();
				m_fBehaviorTimer = 2.f;
			}

			Execute_Behavior(fTimeDelta);
		}
	}


		m_pWeaponColliderCom->Update(XMLoadFloat4x4(m_pModelCom->Get_BoneCombinedMatrix("R_weapon"))* XMLoadFloat4x4(&m_pModelCom->Get_PivotMatrix()) * m_pTransformCom->Get_WorldMatrix());

		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());


		CCollider* pPlayerHand_Collider = ((CCollider*)m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_SPHERE"), 0, CPlayer::PART_RHAND));

		CCollider* pPlayerBodyCol = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_OBB"), 0));
		
		_vector vPlayerColPos = XMVectorSetY(pPlayerBodyCol->Get_Collider_Position(), 0.f);

		_float fPlayerColExt = pPlayerBodyCol->Get_Collider_Radius();

		_vector vColDiff = XMVectorSetY(m_pColliderCom->Get_Collider_Position(),0.f) - vPlayerColPos;
		_float fDist = XMVector3Length(vColDiff).m128_f32[0];

		if (m_pColliderCom->Intersect(pPlayerBodyCol) == true)
		{
			if (fPlayerColExt + m_pColliderCom->Get_Collider_Radius() > fDist)
			{

				m_pTransformCom->Move_To(XMVector3Normalize(vColDiff),fTimeDelta);
			}
		}

		//_float3 vAttackPosition = pPlayerHand_Collider->Get_Collider_Position();

	if (m_pColliderCom->Intersect(pPlayerHand_Collider) == true)
	{

		CCollider::ATTACK_DESC pPlayerAttack = pPlayerHand_Collider->Get_Attack_Desc();

		if (pPlayerHand_Collider->Get_Collider_State() == CCollider::STATE_NORMAL
			&& m_iIgnoreID != pPlayerAttack.iBehaviorID)
		{
		m_iNextAnimIndex = 46;
		m_bIsLoop = false;
		m_tParams.fCurrentHp -= pPlayerAttack.fDamage;
		m_iIgnoreID = pPlayerAttack.iBehaviorID;
		m_fIgnoreTimer = 0.5f;
		pPlayerHand_Collider->Inactivate_Collider();
		
		}

	}	

		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = m_iNextAnimIndex;
		m_pModelCom->Set_Animation(m_iCurrentAnimIndex, m_bIsLoop);

		m_pHp_Bar_Base->Update(m_pTransformCom->Get_State(CTransform::STATE_POSITION), max(m_tParams.fCurrentHp/m_tParams.fMaxHp,0), 0);
}

void CBlack_Knight::LateTick(_float fTimeDelta)
{
	

	Execute_Animation(m_iCurrentAnimIndex, fTimeDelta);
	m_bIsFinished = m_pModelCom->Play_Animation(m_bIsLoop, fTimeDelta);


	if (m_pGameInstance->isIn_Frustum_InWorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

		m_pHp_Bar_Base->LateTick(fTimeDelta);
	}

}

HRESULT CBlack_Knight::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (i ==6 || i ==4 || i == 7)
			continue;

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

HRESULT CBlack_Knight::Add_Component()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		BoundingDesc = {};
	BoundingDesc.vExtents = _float3(1.f, 2.f, 1.0f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	BoundingDesc.vRadians = _float3(0.f, XMConvertToRadians(0.f), 0.f);


	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), (CComponent**)&m_pColliderCom, &BoundingDesc)))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	CBounding_SPHERE::BOUNDING_SPHERE_DESC		WeaponBoundingDesc = {};
	WeaponBoundingDesc.fRadius = 70.f;
	WeaponBoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 110.f);

	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider_SPHERE"), (CComponent**)&m_pWeaponColliderCom, &WeaponBoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlack_Knight::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CBlack_Knight::Determine_Behavior()
{
	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = XMVectorSetY(vPlayerPos - vPosition, 0.f);
	_vector vFlatLook = XMVectorSetY((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), 0.f);

	//Behaviors 

	// Encounter
	if (m_iBehaviorIndex == 7)
	{
		return;
	}
	
	//Player is Not at Front
	if (XMVector3Dot(XMVector3Normalize(vFlatLook), XMVector3Normalize(vToPlayer)).m128_f32[0] < .95f)
	{
		if (XMVector3Dot(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)), XMVector3Normalize(vToPlayer)).m128_f32[0] > 0)
		{
		m_iBehaviorIndex = 4;

		}
		else
		{
		m_iBehaviorIndex = 5;

		}


		return;
	}

	//Monster Has Deviated from Patrol Range
	if (XMVector3Length(m_vInitialPos - vPosition).m128_f32[0] >= 25.f)
	{
		// Return to Original Position
		m_iBehaviorIndex = 6;
		return;
	}

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

void CBlack_Knight::Execute_Behavior(_float fTimeDelta)
{
	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = vPlayerPos - vPosition;
	_float	vDistance = XMVector3Length(vToPlayer).m128_f32[0];
	_vector vFlatLook = XMVectorSetY((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), 0.f);

	m_bMoveFinished = false;



	if (m_iBehaviorIndex == 7 && m_iBehaviorStage == 0)
	{
		if (XMVector3Dot(XMVector3Normalize(vFlatLook), XMVector3Normalize(vToPlayer)).m128_f32[0] < .5f)
		{
			if (XMVector3Dot(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)), XMVector3Normalize(vToPlayer)).m128_f32[0] > 0)
			{

				m_iNextAnimIndex = 43;
				m_bIsLoop = true;

			}
			else
			{
				m_iNextAnimIndex = 42;
				m_bIsLoop = true;
			}

		}
		else
		{


			if (vDistance >= 15.f)
			{
				m_bRun = true;
			}

			if (m_bRun == true)
			{
				m_iNextAnimIndex = 12;
				m_bIsLoop = true;
			}
			else
			{
				m_iNextAnimIndex = 4;
				m_bIsLoop = true;
			}
		if (vDistance <= 8.f && m_bRun==true)
		{
				m_iBehaviorStage++;
				m_bRun = false;
		}

		if (vDistance <= 4.f)
		{
		m_iBehaviorStage++;
		m_bRun = false;
		}

		}
	}
	else if (m_iBehaviorIndex == 7 && m_iBehaviorStage == 1)
	{
		if (XMVector3Dot(XMVector3Normalize(vFlatLook), XMVector3Normalize(vToPlayer)).m128_f32[0] < .95f)
		{
			if (XMVector3Dot(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)), XMVector3Normalize(vToPlayer)).m128_f32[0] > 0)
			{

				m_iNextAnimIndex = 43;
				m_bIsLoop = true;

			}
			else
			{
				m_iNextAnimIndex = 42;
				m_bIsLoop = true;
			}
		}
		else
		{
			if (m_iCurrentAnimIndex == 12)
			{
				m_iNextAnimIndex = 37;
				m_bIsLoop = false;
				m_iBehaviorStage++;
			}
			else
			{
				m_iNextAnimIndex = 29;
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
		}
	}

	//if (m_iBehaviorIndex == 0 && m_iBehaviorStage == 0)
	//{
	//	m_iCurrentAnimIndex = 0;
	//	m_bIsLoop = true;
	//	m_bMoveFinished = true;
	//}

	//if (m_iBehaviorIndex == 1 && m_iBehaviorStage == 0)
	//{
	//	if(m_bIsFinished == true)
	//	m_iBehaviorStage++;

	//	m_pTransformCom->Look_At(m_vInitialPos);
	//	m_iCurrentAnimIndex = 33;
	//	m_bIsLoop = false;
	//}
	//if (m_iBehaviorIndex == 1 && m_iBehaviorStage == 1)
	//{
	//	if (XMVector3Length(vToPlayer).m128_f32[0] <= 3.f)
	//	{
	//	m_pTransformCom->Look_At(m_vInitialPos);
	//	m_iCurrentAnimIndex = 34;
	//	m_bIsLoop = false;

	//	}
	//	else
	//	{
	//		m_iCurrentAnimIndex = 0;
	//		m_bIsLoop = true;
	//		m_bMoveFinished = true;
	//		m_iBehaviorStage = 0;
	//	}
	//	m_iCurrentAnimIndex = 0;
	//	m_bIsLoop = true;
	//	m_bMoveFinished = true;
	//	m_iBehaviorStage = 0;

	//}
	///*if (m_iBehaviorIndex == 2 && m_iBehaviorStage == 0)
	//{

	//	m_pModelCom->Set_Animation(4, true);
	//	m_pTransformCom->Go_Straight(fTimeDelta);

	//	if(XMVector3Length(vToPlayer).m128_f32[0] <= 3.f)
	//	{
	//		m_iCurrentAnimIndex = 4;
	//		m_bIsLoop = false;
	//		m_pModelCom->Set_Animation(4, false);
	//		m_bMoveFinished = true;
	//		m_iBehaviorStage = 0;
	//	}
	//}*/

	//if (m_iBehaviorIndex == 4 && m_iBehaviorStage == 0)
	//{
	//	m_iCurrentAnimIndex = 42;
	//	m_bIsFinished = false;
	//	m_bIsLoop = false;
	//	m_bMoveFinished = true;
	//	return;

	//}
	//if (m_iBehaviorIndex == 5 && m_iBehaviorStage == 0)
	//{
	//	m_iCurrentAnimIndex = 43;
	//	m_bIsFinished = false;
	//	m_bIsLoop = false;
	//	m_bMoveFinished = true;

	//	return;

	//}

	//if (m_iBehaviorIndex == 7 && m_iBehaviorStage == 0)
	//{
	//	m_iCurrentAnimIndex = 2;
	//	m_bIsFinished = false;
	//	m_bIsLoop = false;
	//	m_iBehaviorStage++;
	//	return;

	//}
	//if (m_iBehaviorIndex == 7 && m_iBehaviorStage == 1)
	//{
	//	if (m_bIsFinished)
	//	{
	//	m_iCurrentAnimIndex = 1;
	//	m_iBehaviorIndex = 0; 
	//	m_iBehaviorStage = 0;
	//	m_eCurrentState = STATE_SHIELD;
	//	m_bIsLoop = true;
	//	m_bMoveFinished = true;
	//	}
	//}

	//if (m_iBehaviorIndex == 6 && m_iBehaviorStage == 0)
	//{
	//	//m_pTransformCom->Look_At(m_vInitialPos);
	//	//m_pModelCom->Set_Animation(4,true);
	//	//m_pTransformCom->Go_Straight(fTimeDelta);
	//	if (XMVector3Length(m_vInitialPos - vPosition).m128_f32[0] <= 1.f)
	//	{
	//		m_iCurrentAnimIndex = 0;
	//		m_bIsLoop = true;
	//		m_bMoveFinished = true;
	//		m_bIsAlert = false;
	//		m_iBehaviorStage = 0;
	//		m_iBehaviorIndex = 0;
	//	}
	//}
}

void CBlack_Knight::Execute_Animation(_uint iCurrentAnimIndex, _float fTimeDelta)
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
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta*1.5f);
		m_pTransformCom->Go_Straight(fTimeDelta);
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	case 11:
		break;
	case 12:
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.f);
		m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
		break;
	case 13:
		break;
	case 14:
		break;
	case 15:
		break;
	case 16:
		break;
	case 17:
		break;
	case 18:
		break;
	case 19:
		break;
	case 20:
		break;
	case 21:
		break;
	case 22:
		break;
	case 23:
		break;
	case 24:
		break;
	case 25:
		break;
	case 26:
		break;
	case 27:
		break;
	case 28:
		break;
	case 29:
		if(m_fCurrentAnimPos <= 0.5f)
		{
		m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.2f && vDistance <= 4.f)
		{
			m_iNextAnimIndex = 30;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 30:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
		if (m_fCurrentAnimPos >= 1.2f && vDistance <= 4.f)
		{
			m_iNextAnimIndex = 31;
			m_bIsFinished = false;
			m_bIsLoop = false;
		}
		break;
	case 31:
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Turn_Towards(vToPlayer, fTimeDelta * 2.5f);
		}
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
		if (m_fCurrentAnimPos <= 0.5f)
		{
			m_pTransformCom->Go_Straight(fTimeDelta, 1.5f);
		}
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
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta*2.f);
		break;
	case 43:
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta*2.f);
		break;
	case 44:
		break;
	case 45:
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
	default:
		break;
	}
}

CBlack_Knight* CBlack_Knight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CBlack_Knight* pInstance = new CBlack_Knight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlack_Knight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlack_Knight::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CBlack_Knight* pInstance = new CBlack_Knight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlack_Knight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlack_Knight::Free()
{
	__super::Free();

	if(m_pHp_Bar_Base !=nullptr)
	Safe_Release(m_pHp_Bar_Base);
	
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pWeaponColliderCom);

}
