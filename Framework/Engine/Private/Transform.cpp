#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
{

}

void CTransform::Set_Scaled(const _float3 & vScale)
{
	_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	Set_State(STATE_RIGHT, XMVector3Normalize(WorldMatrix.r[STATE_RIGHT]) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(WorldMatrix.r[STATE_UP]) * vScale.y);
	Set_State(STATE_LOOK , XMVector3Normalize(WorldMatrix.r[STATE_LOOK]) * vScale.z);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX / 2 - g_iWinSizeX * 0.5, -m_fY - m_fSizeY / 2 + g_iWinSizeY * 0.5, 0.f, 1.f));

}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	TRANSFORM_DESC		TransformDesc = *(TRANSFORM_DESC*)pArg;

	m_fSpeedPerSec = TransformDesc.fSpeedPerSec;
	m_fOriginalSpeed = TransformDesc.fSpeedPerSec;
	m_fRotationPerSec = TransformDesc.fRotationPerSec;

	return S_OK;
}

HRESULT CTransform::Bind_ShaderResource(CShader * pShader, const _char * pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, m_WorldMatrix);	
}

HRESULT CTransform::Go_Straight(_float fTimeDelta , _float fSpeedMult)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	m_fSpeedPerSec = m_fOriginalSpeed * fSpeedMult;
	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta, _float fSpeedMult)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	m_fSpeedPerSec = m_fOriginalSpeed * fSpeedMult;
	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK; 
}

HRESULT CTransform::Go_Left(_float fTimeDelta, _float fSpeedMult)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	m_fSpeedPerSec = m_fOriginalSpeed * fSpeedMult;
	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta, _float fSpeedMult)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	m_fSpeedPerSec = m_fOriginalSpeed * fSpeedMult;
	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Up(_float fTimeDelta, _float fSpeedMult)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vUp = Get_State(STATE_UP);

	m_fSpeedPerSec = m_fOriginalSpeed * fSpeedMult;
	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Down(_float fTimeDelta, _float fSpeedMult)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vUp = Get_State(STATE_UP);

	m_fSpeedPerSec = m_fOriginalSpeed * fSpeedMult;
	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec * -fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Orbit_Left(_fvector vTargetPoint, _float fTimeDelta)
{
	//_float fTargetX, fTargetY, fTargetZ;
	//fTargetX = XMVectorGetX(vTargetPoint);
	//fTargetY = XMVectorGetY(vTargetPoint);
	//fTargetZ = XMVectorGetZ(vTargetPoint);
	//_float fSrcPosX, fSrcPosY, fSrcPosZ;
	//fSrcPosX = XMVectorGetX(Get_State(STATE_POSITION));
	//fSrcPosY = XMVectorGetY(Get_State(STATE_POSITION));
	//fSrcPosZ = XMVectorGetZ(Get_State(STATE_POSITION));

	//_vector vDir = vTargetPoint - Get_State(STATE_POSITION);
	//_float fDist = XMVectorGetX(XMVector3Length(vDir));

	//Go_Left(fTimeDelta);

	//_float fDstPosX, fDstPosY, fDstPosZ;
	//fDstPosX = XMVectorGetX(Get_State(STATE_POSITION));
	//fDstPosY = XMVectorGetY(Get_State(STATE_POSITION));
	////fDstPosZ = XMVectorGetZ(Get_State(STATE_POSITION));

	////fDstPosZ * fDstPosZ - fDstPosZ * fTargetZ = pow(fDist, 2) - pow(fTargetY, 2) - pow(fDstPosX - fTargetX, 2) - pow(fTargetZ, 2) - pow(fTargetZ, 2);

	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVector3Normalize(XMVectorSetY((vTargetPoint - vPosition),0.f));
	//_vector		vLook  = XMVector3Normalize(Get_State(STATE_LOOK));
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));


	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec*0.3f * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);


	return S_OK;
}

HRESULT CTransform::Orbit_Right(_float fTargetDist, _float fTimeDelta)
{
	//_float3		vScaled = Get_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);
	_vector		vTargetPoint = XMVector3Normalize(vLook) * fTargetDist;
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition +=  XMVector3Normalize(vRight) * m_fSpeedPerSec * 0.1f * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	Set_State(STATE_LOOK, XMVector3Normalize(vTargetPoint));

	vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMVector3Normalize(vTargetPoint));
	_vector vUp = XMVector3Cross(XMVector3Normalize(vTargetPoint), vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight));
	Set_State(STATE_UP, XMVector3Normalize(vUp));

	return S_OK;
}

HRESULT CTransform::Orbit_Up(_fvector vTargetPoint, _float fTimeDelta)
{

	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVector3Normalize(vTargetPoint - vPosition);
	//_vector		vLook = XMVector3Normalize(Get_State(STATE_LOOK));
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, Get_State(STATE_RIGHT)));

	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec*0.3f * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);


	return S_OK;
}

HRESULT CTransform::Orbit_Down(_float fTargetDist, _float fTimeDelta)
{
	//_float3		vScaled = Get_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);
	_vector		vTargetPoint = XMVector3Normalize(vLook) * fTargetDist;
	_vector		vUp = Get_State(STATE_UP);

	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec * 0.1f * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	Set_State(STATE_LOOK, XMVector3Normalize(vTargetPoint));

	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMVector3Normalize(vTargetPoint));
	vUp = XMVector3Cross(XMVector3Normalize(vTargetPoint), vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight));
	Set_State(STATE_UP, XMVector3Normalize(vUp));

	return S_OK;
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	// XMVector3TransformNormal();
	Set_State(STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::Rotate_From_Camera_Look(_fvector vCamLook, _fvector vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scaled();

	_vector		vLook = XMVector3Normalize(vCamLook) * vScaled.z;

	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vCamLook)) * vScaled.x;
	_vector		vUp = XMVector3Normalize((XMVector3Cross(vCamLook, vRight))) * vScaled.y;


	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	// XMVector3TransformNormal();
	Set_State(STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::Turn_Towards(_fvector vTargetPoint, _float fTimeDelta)
{

	_vector		vTargetDir = XMVectorSetY(vTargetPoint, 0.f);
	_vector		vLookDir = XMVectorSetY(Get_State(STATE_LOOK), 0.f);

	_float fFromCentre = XMVector3Dot(XMVector3Normalize(vTargetDir), XMVector3Normalize(vLookDir)).m128_f32[0];

	if (fFromCentre < 0.95f)
	{
		_vector		vFlatRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir));

		if (XMVector3Dot(XMVector3Normalize(vFlatRight), XMVector3Normalize(vTargetDir)).m128_f32[0] < 0.f)
		{
			Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta );
		}
		else
		{
			Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta );
		}
	}

}

void CTransform::Move_To(_fvector vTargetVec, _float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	vPosition += vTargetVec * fTimeDelta * m_fSpeedPerSec;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Look_At(_fvector vTargetPoint)
{
	_float3		vScaled = Get_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = vTargetPoint - vPosition;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

void CTransform::Turn_Away(_fvector vTargetPoint)
{
	_float3		vScaled = Get_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = vPosition - vTargetPoint;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

void CTransform::Chase_Target(_fvector vTargetPoint, _float fTimeDelta, _float fMargin)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vMoveDir = vTargetPoint - vPosition;

	_float		fDistance = XMVectorGetX(XMVector3Length(vMoveDir));

	if(fDistance >= fMargin)
		vPosition += XMVector3Normalize(vMoveDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Camera_Follow(_fvector vPlayerPos, _float fTimeDelta, _float fMargin, _float CamSpeed)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vMoveDir = vPlayerPos - vPosition;

	vMoveDir = XMVectorSetY(vMoveDir, 0.f);

	_float		fDistance = XMVectorGetX(XMVector3Length(vMoveDir));
	

	if (fDistance >= fMargin)
		vPosition += XMVector3Normalize(vMoveDir) * (m_fSpeedPerSec * ((fDistance-fMargin)/ 3.4f)) * fTimeDelta;
	if (fDistance < fMargin)
		vPosition -= XMVector3Normalize(vMoveDir) * (m_fSpeedPerSec * ((fMargin-fDistance)/ 3.4f)) * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Change_Height(_float fHeight)
{
	_vector vPosition = Get_State(STATE_POSITION);
	
	vPosition += XMVectorSet(0.f, 1.f, 0.f, 0.f) * fHeight;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Lock_On(_fvector vTargetPos, _float fTimeDelta)
{
	_vector		vTargetDir = XMVectorSetY(vTargetPos - Get_State(STATE_POSITION), 0.f);
	_vector		vLookDir = XMVectorSetY(Get_State(STATE_LOOK), 0.f);
	_vector		vFlatRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir));

	_float		fFromCentre = XMVector3Dot(XMVector3Normalize(vLookDir), XMVector3Normalize(vTargetDir)).m128_f32[0];

	if (fFromCentre < 1.f)
	{

		if (XMVector3Dot(XMVector3Normalize(vFlatRight), XMVector3Normalize(vTargetDir)).m128_f32[0] < 0.f)
		{
			Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * (1.f - fFromCentre) * m_fSpeedPerSec);
		}
		else
		{
			Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * (1.f - fFromCentre) * m_fSpeedPerSec);
		}
	}


}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
