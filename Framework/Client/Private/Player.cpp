#include "stdafx.h"
#include "Player.h"
#include "Player_BD.h"
#include "Player_RHand.h"
#include "Player_LHand.h"
#include "Player_AM.h"
#include "Player_HD.h"
#include "Player_LG.h"
#include "Sound_Manager.h"

#include "GameInstance.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
	, m_tPlayerParams(rhs.m_tPlayerParams)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	m_tPlayerParams.fMaxHp = { 590.f };
	m_tPlayerParams.fCurrentHp = { 590.f };
	m_tPlayerParams.fMaxStamina = { 95.f };
	m_tPlayerParams.fCurrentStamina = { 95.f };
	m_tPlayerParams.fMaxFp = { 93.f };
	m_tPlayerParams.fCurrentFp = { 93.f };
	m_tPlayerParams.fRigidity = { 0.0f };
	m_tPlayerParams.fFind = { 107.f };
	m_tPlayerParams.fCurrentWeight = { 0.f };
	m_tPlayerParams.fMaxWeight = { 50.f };

	m_tPlayerParams.iLevel = { 9 };
	m_tPlayerParams.iVigor = { 12 };
	m_tPlayerParams.iAttunement = { 10 };
	m_tPlayerParams.iEndurance = { 11 };
	m_tPlayerParams.iVitality = { 15 };
	m_tPlayerParams.iStrength = { 13 };
	m_tPlayerParams.iDexterity = { 12 };
	m_tPlayerParams.iIntelligence = { 9 };
	m_tPlayerParams.iFaith = { 9 };
	m_tPlayerParams.iLuck = { 7 };
	m_tPlayerParams.iMemorySlots = { 1 };

	m_tPlayerParams.iSouls = { 0 };

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	m_strObjName = TEXT("Player");

	if (pArg != nullptr)
	{
		m_tPlayerParams = *(PLAYER_PARAM*)pArg;
	}

	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(7.f, 90.0f))))
		return E_FAIL;

 	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(Add_Parts()))
		return E_FAIL;


	dynamic_cast<CCollider*>(m_Parts[PART_RHAND]->Find_Component(TEXT("Com_Collider_SPHERE")))->Ready_Attack_Behavior(&m_tPlayer_Attack);
	m_pColliderCom->Activate_Collider();
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(30));
	//m_pTransformCom->Set_Scaled(_float3{ 0.02f,0.02f,0.02f });
		

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{

	SetUpOnTerrain();

	CTransform* pCameraTransform = (CTransform*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Camera"), TEXT("Com_Transform"));
	pCameraTransform->Get_State(CTransform::STATE_LOOK);

	_vector vCameraCross = XMVector3Cross( pCameraTransform->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_UP));
	_vector vCameraLook = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vCameraCross );
	_vector vCrossUp = XMVector3Cross(vCameraCross, vCameraLook);

	if (m_bInputCheck == false)
	{
		m_fInputDelay -= fTimeDelta;

		if (m_fInputDelay <= 0.f)
		{
			m_fInputDelay = 0.f;
			m_bInputCheck = true;
			m_bCancelEnable = true;
		}

	}
	else
	{
		if (m_bCancelEnable == true && m_bIsFinished == false)
		{
#pragma region CancelAble Moves

			if (m_bIsFinished == true)
				m_bCancelEnable = false;

			if (m_bCancelEnable == true)
			{

				if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
					&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_FRONT_LEFT;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;
				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
					&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_FRONT_RIGHT;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;
				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
					&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_BACK_LEFT;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;
				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
					&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_BACK_RIGHT;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;
				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_FRONT;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;

				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_BACK;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;


				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_LEFT;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;

				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
					m_bIsLoop = true;
					m_eCurrentDir = DIR_RIGHT;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 78;
				}


				if (m_pGameInstance->Get_DIMouseState(MKS_LBUTTON))
				{
					if (m_iCurrentAnimIndex == 63)
					{

						if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
						{
							m_eCurrentDir = DIR_FRONT_LEFT;
							m_eCurrentState = STATE_MOVE;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
						{
							m_eCurrentDir = DIR_FRONT_RIGHT;
							m_eCurrentState = STATE_MOVE;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
						{
							m_eCurrentDir = DIR_BACK_LEFT;
							m_eCurrentState = STATE_MOVE;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
						{
							m_eCurrentDir = DIR_BACK_RIGHT;
							m_eCurrentState = STATE_MOVE;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
						{
							m_eCurrentDir = DIR_FRONT;
							m_eCurrentState = STATE_MOVE;

						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
						{
							m_eCurrentDir = DIR_BACK;
							m_eCurrentState = STATE_MOVE;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
						{
							m_eCurrentDir = DIR_LEFT;
							m_eCurrentState = STATE_MOVE;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
						{
							m_eCurrentDir = DIR_RIGHT;
							m_eCurrentState = STATE_MOVE;
						}

						m_bIsLoop = false;
						m_iNextAnimIndex = 64;
						m_eCurrentState = STATE_ATTACK;
						m_bInputCheck = false;
						m_fInputDelay = 0.7f;
					}
					else
					{

						if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
						{
							m_eCurrentDir = DIR_FRONT_LEFT;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
						{
							m_eCurrentDir = DIR_FRONT_RIGHT;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
						{
							m_eCurrentDir = DIR_BACK_LEFT;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
							&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
						{
							m_eCurrentDir = DIR_BACK_RIGHT;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
						{
							m_eCurrentDir = DIR_FRONT;

						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
						{
							m_eCurrentDir = DIR_BACK;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
						{
							m_eCurrentDir = DIR_LEFT;
						}
						else if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
						{
							m_eCurrentDir = DIR_RIGHT;
						}

						m_bIsLoop = false;
						m_iNextAnimIndex = 63;
						m_eCurrentState = STATE_ATTACK;
						m_bInputCheck = false;
						m_fInputDelay = 0.7f;

					}
				}


				if (m_pGameInstance->Get_Key_Up(VK_SPACE) && m_iCurrentAnimIndex != 76)
				{

					if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
						&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
					{
						m_eCurrentDir = DIR_FRONT_LEFT;
					}
					else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
						&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
					{
						m_eCurrentDir = DIR_FRONT_RIGHT;
					}
					else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
						&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
					{
						m_eCurrentDir = DIR_BACK_LEFT;
					}
					else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
						&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
					{
						m_eCurrentDir = DIR_BACK_RIGHT;
					}
					else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
					{
						m_eCurrentDir = DIR_FRONT;

					}
					else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
					{
						m_eCurrentDir = DIR_BACK;
					}
					else if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
					{
						m_eCurrentDir = DIR_LEFT;
					}
					else if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
					{
						m_eCurrentDir = DIR_RIGHT;
					}

					m_iNextAnimIndex = 111;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_bInputCheck = false;
					m_bIsLoop = false;
					m_fInputDelay = .7;
				}
			}

#pragma endregion
		}
		else
		{
			m_eCurrentDir = DIR_FRONT;
			m_eCurrentState = STATE_IDLE;
			m_bIsLoop = true;
			m_iNextAnimIndex = 75;


			if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
				&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
			{

				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_FRONT_LEFT;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80
				&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
			{
				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_FRONT_RIGHT;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
				&& m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
			{
				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_BACK_LEFT;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80
				&& m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
			{
				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_BACK_RIGHT;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
			{
				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_FRONT;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;

			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
			{
				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_BACK;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;

			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
			{
				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_LEFT;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;

			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
			{
				if (m_bisRunning)
				{
					m_pTransformCom->Go_Straight(fTimeDelta, 2.f);
				}
				else
				{
					m_pTransformCom->Go_Straight(fTimeDelta);
				}
				m_bIsLoop = true;
				m_eCurrentDir = DIR_RIGHT;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_iNextAnimIndex = 78;
			}

			if (m_pGameInstance->Get_Key_Press(VK_SPACE) && (m_iCurrentAnimIndex == 78 || m_iCurrentAnimIndex == 76))
			{
				m_fSInputDelay -= fTimeDelta;
				if (m_fSInputDelay < 0.f)
				{

					m_bisRunning = true;
					m_eCurrentState = STATE_MOVE;
					SetDirection(m_eCurrentDir, fTimeDelta);
					m_iNextAnimIndex = 76;
					m_bIsLoop = true;
				}
			}

#pragma endregion

			if (m_pGameInstance->Get_Key_Up(VK_SPACE) && m_iCurrentAnimIndex != 76)
			{
				m_iNextAnimIndex = 111;
				m_eCurrentState = STATE_MOVE;
				SetDirection(m_eCurrentDir, fTimeDelta);
				m_bInputCheck = false;
				m_bIsLoop = false;
				m_bIsFinished = false;
				m_fInputDelay = .7f;
				m_fSInputDelay = 0.3f;
			}

			if (m_pGameInstance->Get_DIMouseState(MKS_LBUTTON))
			{
				if (m_iCurrentAnimIndex != 64)
				{
					m_bIsLoop = false;
					m_iNextAnimIndex = 63;
					m_eCurrentState = STATE_ATTACK;
					m_bInputCheck = false;
					m_fInputDelay = 0.6f;
					m_bIsFinished = false;
				}

			}

		}
		if (m_iCurrentAnimIndex == 76 && m_iNextAnimIndex != 76)
		{
			m_bisRunning = false;
			m_fSInputDelay = 0.3f;
		}

		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = m_iNextAnimIndex;
		m_pModelCom->Set_Animation(m_iCurrentAnimIndex, m_bIsLoop);
		if (m_iCurrentAnimIndex != m_iPrevAnimIndex)
		{
			m_bResetTimer = true;
		}

		if (m_iCurrentAnimIndex == 111)
		{
		m_iNextAnimIndex = 75;

		}


	}


		if (m_pGameInstance->Get_DIKeyState(DIK_P) & 0x80)
		{
			m_tPlayerParams.fCurrentHp -= 10.f;
		}



		_uint iMonsterNum = m_pGameInstance->Get_NumObjects(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Monster"));
	for (_int i = 0; i < iMonsterNum; i++)
	{
		CCollider* pEnemeyAttackCollider = (CCollider*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Monster"), TEXT("Com_Collider_SPHERE"), i);
		
		CCollider* pMonsterBodyCol = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Monster"), TEXT("Com_Collider_OBB"), i));
		
		if (pMonsterBodyCol != nullptr)
		{
		_vector vMonsterColPos = XMVectorSetY(pMonsterBodyCol->Get_Collider_Position(), 0.f);
				 
		_float fMonsterColExt = pMonsterBodyCol->Get_Collider_Radius();

		_vector vColDiff = XMVectorSetY(m_pColliderCom->Get_Collider_Position(), 0.f) - vMonsterColPos;
		
		_float fDist = XMVector3Length(vColDiff).m128_f32[0];

			if (m_pColliderCom->Intersect(pMonsterBodyCol) == true)
			{
				if (fMonsterColExt + m_pColliderCom->Get_Collider_Radius() > fDist)
				{
					
					m_pTransformCom->Move_To(XMVector3Normalize(vColDiff),fTimeDelta);
				}
			}
		}



		if (pEnemeyAttackCollider != nullptr)
		{

			if (m_pColliderCom->Intersect(pEnemeyAttackCollider) == true)
			{

				CCollider::ATTACK_DESC pMonsterAttack = pEnemeyAttackCollider->Get_Attack_Desc();

				_vector vAttackPosition = pEnemeyAttackCollider->Get_Collider_Position();
				_vector vDirection = m_pColliderCom->Get_Collider_Position() - vAttackPosition;
				// Positive = From Right Negative = From Left
				_float fRightDotProduct = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), vDirection));
				// Positive = From Front Negative = From Back
				_float fLookDotProduct = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK), vDirection));

				if (pEnemeyAttackCollider->Get_Collider_State() == CCollider::STATE_NORMAL)
				{

					if (pMonsterAttack.eAttackType == CCollider::ATK_HEAVY)
					{
						//Heavy Hit motion
						if (fLookDotProduct >= 0)
						{
							//Attacked from Front
							CSound_Manager::Get_Instance()->PlaySound(L"Player_Hit.wav", CSound_Manager::SOUND_PLAYER_HIT1, 0.5f);
							m_iNextAnimIndex = 114;
							m_bInputCheck = false;
							m_fInputDelay = 0.5f;
							m_bIsLoop = false;
							m_tPlayerParams.fCurrentHp -= pMonsterAttack.fDamage;
							pEnemeyAttackCollider->Inactivate_Collider();
							m_pModelCom->Set_Animation(m_iNextAnimIndex, m_bIsLoop);

						}
						else if (fLookDotProduct <0)
						{
							//Attacked from Back
							CSound_Manager::Get_Instance()->PlaySound(L"Player_Hit.wav", CSound_Manager::SOUND_PLAYER_HIT1, 0.5f);
							m_iNextAnimIndex = 114;
							m_bInputCheck = false;
							m_fInputDelay = 0.5f;
							m_bIsLoop = false;
							m_tPlayerParams.fCurrentHp -= pMonsterAttack.fDamage;
							pEnemeyAttackCollider->Inactivate_Collider();
							m_pModelCom->Set_Animation(m_iNextAnimIndex, m_bIsLoop);

						}

					}
					else if (pMonsterAttack.eAttackType == CCollider::ATK_NORMAL)
					{
						//Normal Hit motion
						CSound_Manager::Get_Instance()->PlaySound(L"Player_Hit.wav", CSound_Manager::SOUND_PLAYER_HIT1, 0.5f);
						m_iNextAnimIndex = 115;
						m_bInputCheck = false;
						m_fInputDelay = 0.5f;
						m_bIsLoop = false;
						m_tPlayerParams.fCurrentHp -= pMonsterAttack.fDamage;
						pEnemeyAttackCollider->Inactivate_Collider();

					}
					else if (pMonsterAttack.eAttackType == CCollider::ATK_LIGHT)
					{
						//Light Hit motion
						CSound_Manager::Get_Instance()->PlaySound(L"Player_Hit.wav", CSound_Manager::SOUND_PLAYER_HIT1, 0.5f);
						m_iNextAnimIndex = 115;
						m_bInputCheck = false;
						m_fInputDelay = 0.5f;
						m_bIsLoop = false;
						m_tPlayerParams.fCurrentHp -= pMonsterAttack.fDamage;
						pEnemeyAttackCollider->Inactivate_Collider();

					}
					else if (pMonsterAttack.eAttackType == CCollider::ATK_DOWN)
					{
						//Down Hit motion
						CSound_Manager::Get_Instance()->PlaySound(L"Player_Hit.wav", CSound_Manager::SOUND_PLAYER_HIT1, 0.5f);
						m_iNextAnimIndex = 115;
						m_bInputCheck = false;
						m_fInputDelay = 0.5f;
						m_bIsLoop = false;
						m_tPlayerParams.fCurrentHp -= pMonsterAttack.fDamage;
						pEnemeyAttackCollider->Inactivate_Collider();

					}
					else if (pMonsterAttack.eAttackType == CCollider::ATK_AIRBOURN)
					{
						//AirBourn Hit motion
						CSound_Manager::Get_Instance()->PlaySound(L"Player_Hit.wav", CSound_Manager::SOUND_PLAYER_HIT1, 0.5f);
						m_iNextAnimIndex = 118;
						m_bInputCheck = false;
						m_fInputDelay = 0.5f;
						m_bIsLoop = false;
						m_tPlayerParams.fCurrentHp -= pMonsterAttack.fDamage;
						pEnemeyAttackCollider->Inactivate_Collider();

					}

				}

			}
		}

	}


	for (size_t i = 0; i < PART_END; i++)
		m_Parts[i]->Tick(fTimeDelta);

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	
}

void CPlayer::LateTick(_float fTimeDelta)
{
	Execute_Animation(m_iCurrentAnimIndex, fTimeDelta);
	m_bIsFinished = m_pModelCom->Play_Animation(m_bIsLoop, fTimeDelta);
	
	for (size_t i = 0; i < PART_END; i++)
		m_Parts[i]->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//for (size_t i = 0; i < iNumMeshes; i++)
	//{

	//	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
	//		return E_FAIL;
	//	/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
	//		return E_FAIL;*/


	//	if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
	//		return E_FAIL;

	//	if (FAILED(m_pShaderCom->Begin(0)))
	//		return E_FAIL;

	//	if (FAILED(m_pModelCom->Render(i)))
	//		return E_FAIL;
	//}


//#pragma region 소울 출력
//
//	wsprintf(m_szSouls, TEXT("%d"), m_iSouls);
//
//	_float fLength = wcslen(m_szSouls);
//
//	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(m_szSouls), _float2(1195.f-fLength*10, 650.f), 0.5f, Colors::White);
//
//#pragma endregion

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

CComponent* CPlayer::Find_Component(const wstring& strComTag, _int iPartObjectIndex)
{
	if (iPartObjectIndex >= PART_END)
		return nullptr;

	if (-1 == iPartObjectIndex)
		return __super::Find_Component(strComTag);

	else
		return m_Parts[iPartObjectIndex]->Find_Component(strComTag);
}

_bool CPlayer::Restore_Hp(_float fHp)
{
	 if (m_tPlayerParams.fCurrentHp < m_tPlayerParams.fMaxHp) 
	 {
		 m_tPlayerParams.fCurrentHp += fHp;
		 if (m_tPlayerParams.fCurrentHp > m_tPlayerParams.fMaxHp)
		 {
			 m_tPlayerParams.fCurrentHp = m_tPlayerParams.fMaxHp;
		 }
		 return true;
	 }

	 return false;
}

HRESULT CPlayer::Add_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(static_cast<LEVELID>(m_pGameInstance->Get_NextLevelIndex()), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Model_Tarnished"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		BoundingDesc = {};
	BoundingDesc.vExtents = _float3(1.f, 1.8f, 1.0f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	BoundingDesc.vRadians = _float3(0.f, XMConvertToRadians(0.f), 0.f);


	if (FAILED(__super::Add_Components((LEVELID)m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), (CComponent**)&m_pColliderCom, &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Add_Parts()
{
	CGameObject* pPart = nullptr;

	/* For.RHand */
	CPlayer_RHand::HAND_DESC_R		HandDesc = {};
	HandDesc.pParentTransform = m_pTransformCom;
	HandDesc.PivotMatrix = m_pModelCom->Get_PivotMatrix();
	HandDesc.pSocket = m_pModelCom->Get_BoneCombinedMatrix("R_Weapon");

	pPart = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Player_RHand"), &HandDesc);
	if (nullptr == pPart)
		return E_FAIL;
	m_Parts[PART_RHAND] = pPart;

	/* For.Body */
	CPlayer_BD::BODY_DESC		BodyDesc = {};
	BodyDesc.pParentTransform = m_pTransformCom;
	BodyDesc.PivotMatrix = m_pModelCom->Get_PivotMatrix();
	BodyDesc.pSocket = m_pModelCom->Get_BoneCombinedMatrix("SpineArmor2");
	BodyDesc.pParentModel = m_pModelCom;

	pPart = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_BD1950"), &BodyDesc);
	if (nullptr == pPart)
		return E_FAIL;
	m_Parts[PART_BODY] = pPart;

	/* For.Leg */
	CPlayer_LG::LEG_DESC		LegDesc = {};
	LegDesc.pParentTransform = m_pTransformCom;
	LegDesc.PivotMatrix = m_pModelCom->Get_PivotMatrix();
	LegDesc.pSocket = m_pModelCom->Get_BoneCombinedMatrix("RootPos");
	LegDesc.pParentModel = m_pModelCom;

	pPart = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_LG1950"), &LegDesc);
	if (nullptr == pPart)
		return E_FAIL;
	m_Parts[PART_LEG] = pPart;

	/* For.Arm */
	CPlayer_AM::ARM_DESC		ArmDesc = {};
	ArmDesc.pParentTransform = m_pTransformCom;
	ArmDesc.PivotMatrix = m_pModelCom->Get_PivotMatrix();
	ArmDesc.pSocket = m_pModelCom->Get_BoneCombinedMatrix("RootPos");
	ArmDesc.pParentModel = m_pModelCom;

	pPart = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_AM1950"), &ArmDesc);
	if (nullptr == pPart)
		return E_FAIL;
	m_Parts[PART_ARM] = pPart;

	/* For.Head */
	CPlayer_HD::HEAD_DESC		HeadDesc = {};
	HeadDesc.pParentTransform = m_pTransformCom;
	HeadDesc.PivotMatrix = m_pModelCom->Get_PivotMatrix();
	HeadDesc.pSocket = m_pModelCom->Get_BoneCombinedMatrix("Head");

	pPart = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_HD1950"), &HeadDesc);
	if (nullptr == pPart)
		return E_FAIL;
	m_Parts[PART_HEAD] = pPart;

	return S_OK;
}

HRESULT CPlayer::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CPlayer::SetDirection(PLAYER_DIR eDirection, _float fTimeDelta)
{
	_float fAngle = 45.f * eDirection;
	if (fAngle > 180.f)
		fAngle = fAngle - 360.f;

	if (abs(m_fCurrentAngle - fAngle) > 185)
	{
		if (m_fCurrentAngle > 0)
		{
		m_fCurrentAngle = m_fCurrentAngle - 360.f;
		}
		else
		{
		m_fCurrentAngle = m_fCurrentAngle + 360.f;
		}
	}

	_float4 vCamLook = m_pGameInstance->Get_CamLook();

	XMStoreFloat4(&vCamLook,XMVectorSetY(XMLoadFloat4(&vCamLook),0.f));

	_vector vCamRight = XMVector3Cross(XMLoadFloat4(&vCamLook),XMVectorSet(0.f, 1.f, 0.f, 0.f));
	_vector vCamHoriz = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vCamRight);

	if (m_fCurrentAngle < fAngle)
	{
		m_fCurrentAngle += fTimeDelta * 50*30;
		m_pTransformCom->Rotate_From_Camera_Look(vCamHoriz,XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(min(m_fCurrentAngle, fAngle)));
	}
	if (m_fCurrentAngle > fAngle)
	{
		m_fCurrentAngle -= fTimeDelta * 50*30;
		m_pTransformCom->Rotate_From_Camera_Look(vCamHoriz,XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(max(m_fCurrentAngle, fAngle)));
	}
}

void CPlayer::Execute_Animation(_uint iCurrentAnimIndex, _float fTimeDelta)
{
	m_fCurrentAnimPos += fTimeDelta;

	CCollider* pHandCollider = (CCollider*)m_Parts[PART_RHAND]->Find_Component(TEXT("Com_Collider_SPHERE"));

	if (m_bResetTimer == true)
	{
		m_fCurrentAnimPos =0.f;
		m_bResetTimer = false;
	}

	if (m_iCurrentAnimIndex == 75)
	{
		m_pColliderCom->Activate_Collider();
	}


	if (m_iCurrentAnimIndex == 111 && m_bInputCheck == false)
	{
		m_pTransformCom->Go_Straight(fTimeDelta,2.f);

		m_pColliderCom->Inactivate_Collider();

		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.35f)
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("FootStep_Roll_Land (2).wav"), CSound_Manager::SOUND_PLAYER_EFFECT2, 0.4f);
		}
		if (m_fCurrentAnimPos >= 0.6f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_PLAYER_EFFECT2);
			m_pColliderCom->Activate_Collider();
			m_fCurrentAnimPos = 0.f;
		}
	}

	if (m_iCurrentAnimIndex == 76)
	{
		if (m_fCurrentAnimPos >= 0.05f && m_fCurrentAnimPos < 0.1f)
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("FootStep(0).wav"), CSound_Manager::SOUND_PLAYER_EFFECT3, 0.3f);
		}
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.33f)
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("FootStep(1).wav"), CSound_Manager::SOUND_PLAYER_EFFECT4, 0.3f);
		}
		if (m_fCurrentAnimPos >= 0.5f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_PLAYER_EFFECT3);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_PLAYER_EFFECT4);
			m_fCurrentAnimPos = 0.f;
		}
	}

	if (m_iCurrentAnimIndex == 78)
	{
		if (m_fCurrentAnimPos >= 0.1f && m_fCurrentAnimPos < 0.15f)
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("FootStep(0).wav"), CSound_Manager::SOUND_PLAYER_EFFECT3, 0.3f);
		}
		if (m_fCurrentAnimPos >= 0.5f && m_fCurrentAnimPos < 0.55f)
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("FootStep(1).wav"), CSound_Manager::SOUND_PLAYER_EFFECT4, 0.3f);
		}
		if (m_fCurrentAnimPos >= 0.8f)
		{
			m_fCurrentAnimPos = 0.f;
		}
	}
	if (m_iCurrentAnimIndex == 64)
	{
		
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.6f)
		{
			m_pTransformCom->Go_Straight(fTimeDelta);
			CSound_Manager::Get_Instance()->PlaySound(TEXT("Moonlight_S0_Normal_Attack_Swing_1.wav"), CSound_Manager::SOUND_PLAYER_EFFECT1, 0.5f);
		}

		if (m_fCurrentAnimPos >= 0.4f && m_fCurrentAnimPos < 0.6f)
		{
			m_tPlayer_Attack.eAttackType = CCollider::ATK_NORMAL;
			m_tPlayer_Attack.fDamage = 212.f;
			m_tPlayer_Attack.fPostureDamage = 10.f;
			m_tPlayer_Attack.iBehaviorID = 64;
			pHandCollider->Activate_Collider();
		}
		if (m_fCurrentAnimPos > 0.65f)
		{
			m_tPlayer_Attack = {};
			pHandCollider->Inactivate_Collider();
		}
	}
	if (m_iCurrentAnimIndex == 63)
	{
		if (m_fCurrentAnimPos >= 0.3f && m_fCurrentAnimPos < 0.6f)
		{
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if (m_fCurrentAnimPos >= 0.4f && m_fCurrentAnimPos < 0.6f)
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("Moonlight_S0_Normal_Attack_Swing_2.wav"), CSound_Manager::SOUND_PLAYER_EFFECT1, 0.5f);
			m_tPlayer_Attack.eAttackType = CCollider::ATK_NORMAL;
			m_tPlayer_Attack.fDamage = 204.f;
			m_tPlayer_Attack.fPostureDamage = 10.f;
			m_tPlayer_Attack.iBehaviorID = 63;
			pHandCollider->Activate_Collider();
		}
		if (m_fCurrentAnimPos > 0.65f)
		{
			m_tPlayer_Attack = {};
			pHandCollider->Inactivate_Collider();
		}
	}
}

void CPlayer::SetUpOnTerrain()
{
	
	CModel* pModel = (CModel*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_BackGround"), TEXT("Com_Model"), 1);
	CTransform* pTerrainTransform = (CTransform*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_BackGround"), TEXT("Com_Transform"),1);

	_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	 

	_bool isRev;

	if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_GUNDYR)
	{
		isRev = false;
	}
	else
	{
		isRev = true;
	}

		vWorldPos = XMVectorSetY(vWorldPos, pModel->Compute_Height(vWorldPos, pTerrainTransform, isRev));
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWorldPos);

}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{

	__super::Free();

	for(size_t i = 0 ; i<PART_END; i++)
	Safe_Release(m_Parts[i]);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
}
