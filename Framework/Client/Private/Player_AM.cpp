#include "stdafx.h"
#include "..\Public\Player_AM.h"

#include "GameInstance.h"

CPlayer_AM::CPlayer_AM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlayer_AM::CPlayer_AM(const CPlayer_AM & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CPlayer_AM::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_AM::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;

	m_pParentTransform = ((ARM_DESC*)pArg)->pParentTransform;
	m_pParentModelCom = ((ARM_DESC*)pArg)->pParentModel;
	m_PivotMatrix = ((ARM_DESC*)pArg)->PivotMatrix;
	//m_pSocketMatrix = ((ARM_DESC*)pArg)->pSocket;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;
	

	//m_pTransformCom->Set_Scaled(_float3(0.8f, 0.8f, 0.8f));
	//m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(270.0f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.6f, 3.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer_AM::Tick(_float fTimeDelta)
{

}

void CPlayer_AM::LateTick(_float fTimeDelta)
{
	m_pModelCom->Bind_Parts(m_pParentModelCom->Get_ModelBones());

	_matrix SocketMatrix = XMMatrixIdentity();//XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

	XMStoreFloat4x4(&m_WorldMatrix,
		m_pTransformCom->Get_WorldMatrix() *
		SocketMatrix *
		XMLoadFloat4x4(&m_PivotMatrix) *
		m_pParentTransform->Get_WorldMatrix());
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CPlayer_AM::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CPlayer_AM::Render_Shadow()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_WorldMatrix)))
		return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(-20.f, 20.f, -20.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (_float)g_iWinSizeX / g_iWinSizeY, 0.1f, 300.f));

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", ProjMatrix)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CPlayer_AM::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Model_AM1950"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CPlayer_AM::Bind_ShaderResources()
{
	_float fFar = m_pGameInstance->Get_CamFar();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fCamFar", &fFar, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	
	return S_OK;
}

CPlayer_AM * CPlayer_AM::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CPlayer_AM* pInstance = new CPlayer_AM(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_AM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_AM::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CPlayer_AM* pInstance = new CPlayer_AM(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_AM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_AM::Free()
{
	__super::Free();
	
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
