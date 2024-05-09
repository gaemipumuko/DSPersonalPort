#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "Player.h"

CCamera_Free::CCamera_Free(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	m_strObjName = TEXT("Camera");

	CAMERA_FREE_DESC*	pCamera_Free_Desc = (CAMERA_FREE_DESC*)pArg;

	if (FAILED(__super::Initialize(&pCamera_Free_Desc->CameraDesc)))
		return E_FAIL;

	m_fMouseSensor = pCamera_Free_Desc->fMouseSensor;
	m_fRotation = pCamera_Free_Desc->CameraDesc.GameObjectDesc.TransformDesc.fRotationPerSec;

	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{

	CPlayer* pPlayer = (CPlayer*)m_pGameInstance->Get_Object(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Player"));

	CTransform* pPlayerTransformCom = (CTransform*)m_pGameInstance->Get_Object(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Player"))->Find_Component(TEXT("Com_Transform"));
	//m_pTransformCom->Camera_Follow((dynamic_cast<CTransform*>(m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player"))->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)), fTimeDelta, 5.f, 5.f);

	_float fCameraDist = XMVectorGetX(XMVector3Length((pPlayerTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION))));

	_long	MouseMove = 0;


	if (MouseMove = m_pGameInstance->Get_DIMouseMove(MMS_X))
	{
			
			//m_pTransformCom->Orbit_Left(pPlayerTransformCom->Get_State(CTransform::STATE_POSITION), MouseMove * m_fMouseSensor * -fTimeDelta);
			//->Look_At(XMVectorSetY(pPlayerTransformCom->Get_State(CTransform::STATE_POSITION),3.f));
			//m_pTransformCom->Go_Right(fTimeDelta);

			//m_pTransformCom->Look_At(m_vCameraCentre);
			m_pTransformCom->Orbit_Right(5.f, MouseMove * m_fMouseSensor * -fTimeDelta);
			//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * 0.1f * fTimeDelta);
			//m_pTransformCom->Turn((dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY,TEXT("Layer_Player"),TEXT("Com_Transform"))))->Get_State(CTransform::STATE_UP), MouseMove * m_fMouseSensor * fTimeDelta);
			//SetUp_Camera(fCameraDist);
	}


	if (MouseMove = m_pGameInstance->Get_DIMouseMove(MMS_Y))
	{
			m_fCameraAngleY += XMConvertToDegrees(m_fRotation * MouseMove * m_fMouseSensor * fTimeDelta);

		if (m_fCameraAngleY <= 80 && m_fCameraAngleY >= -80)
		{
			m_pTransformCom->Orbit_Down(1.f, MouseMove * m_fMouseSensor * fTimeDelta);
			//m_pTransformCom->Orbit_Up(m_vCameraCentre, MouseMove * m_fMouseSensor * fTimeDelta);
		//	m_pTransformCom->Look_At(XMVectorSetY(pPlayerTransformCom->Get_State(CTransform::STATE_POSITION), 3.f));
			//m_pTransformCom->Look_At(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK))*4.6);


			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * m_fMouseSensor* 0.3f * fTimeDelta);
			//m_pTransformCom->Go_Up(MouseMove* m_fMouseSensor* fTimeDelta);
			//SetUp_Camera(fCameraDist);
		}
		else if (m_fCameraAngleY > 80)
		{
			//m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -MouseMove * m_fMouseSensor * fTimeDelta);
			m_fCameraAngleY = 80;
			//SetUp_Camera(fCameraDist);
		}
		else if (m_fCameraAngleY < -80)
		{
			//m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -MouseMove * m_fMouseSensor * fTimeDelta);
			m_fCameraAngleY = -80;
			//SetUp_Camera(fCameraDist);
		}
	}


		m_pTransformCom->Lock_On(pPlayerTransformCom->Get_State(CTransform::STATE_POSITION), fTimeDelta);

		m_pTransformCom->Camera_Follow(pPlayerTransformCom->Get_State(CTransform::STATE_POSITION),fTimeDelta,5.2f,5.f);


		if (m_fPastPlayerY != pPlayerTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1])
		{
			m_pTransformCom->Change_Height(pPlayerTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1]- m_fPastPlayerY);
		}

		m_fPastPlayerY = pPlayerTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1];
		//_vector vLook = { 0.f,0.f,0.f,0.f };
		//_vector vCurrentLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		//vLook = XMVectorSetY(vLook,m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[1]);
		//vLook = XMVectorSetZ(vLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[2]);
		//_vector vAxis = { 0.f,0.f,1.f,0.f };
		//if (XMVector3Dot(XMVector3Normalize(vLook), vAxis).m128_f32[0] >= 0)
		//{
		//m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * m_fMouseSensor * fTimeDelta);
		//}
		//else if(XMVector3Dot(XMVector3Normalize(vLook), vAxis).m128_f32[0] < 0)
		//{
		//	if (vLook.m128_f32[1] > 0 )
		//	{
		//		
		//		vCurrentLook = XMVectorSetY(vCurrentLook, 0.99);

		//		m_pTransformCom->Set_State(CTransform::STATE_LOOK,vCurrentLook);
		//	}
		//	if (vLook.m128_f32[1] < 0)
		//	{
		//		vCurrentLook = XMVectorSetY(vCurrentLook, -0.99);
		//		m_pTransformCom->Set_State(CTransform::STATE_LOOK,vCurrentLook);
		//	}
		//	}

	//m_pTransformCom->Look_At((dynamic_cast<CTransform*>(m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player"))->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)));

	__super::Tick(fTimeDelta);
}

void CCamera_Free::LateTick(_float fTimeDelta)
{
	//dynamic_cast<CTransform*>(m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player"))->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pGameInstance->Get_CamPosition()));
	
}

void CCamera_Free::SetUp_Camera(_float fCamDist)
{

 	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (dynamic_cast<CTransform*>(m_pGameInstance->Get_Object(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Player"))->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)));

	m_fPastPlayerY = (dynamic_cast<CTransform*>(m_pGameInstance->Get_Object(
		m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Player"))->Find_Component(TEXT("Com_Transform"))))->Get_State(CTransform::STATE_POSITION).m128_f32[1];

	if (fCamDist == 0.f)
	{
	m_pTransformCom->Go_Backward(.18f);
	m_pTransformCom->Go_Up(.1f);
	m_vCameraCentre = (dynamic_cast<CTransform*>(m_pGameInstance->Get_Object(
		m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Player"))->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION))
		- (m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	else
	{
	m_pTransformCom->Go_Up(.1f);
	m_pTransformCom->Go_Backward(fCamDist/ m_pTransformCom->Get_CurrentSpeed());
	}
}

CCamera_Free* CCamera_Free::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

}
