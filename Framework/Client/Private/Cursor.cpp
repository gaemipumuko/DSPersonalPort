#include "stdafx.h"
#include "..\Public\Cursor.h"

#include "GameInstance.h"

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CMouse::CMouse(const CMouse & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CMouse::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMouse::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_strObjName = TEXT("Mouse");

	m_fSizeX = 10.f;
	m_fSizeY = 10.f;

	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CMouse::Tick(_float fTimeDelta)
{

	//SetCursorPos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);

	m_iXmove += m_pGameInstance->Get_DIMouseMove(MMS_X);
	m_iYmove += m_pGameInstance->Get_DIMouseMove(MMS_Y);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(g_iWinSizeX*0.5f + m_iXmove, g_iWinSizeY*(-0.5f) - m_iYmove, 0.f, 1.f));

}

void CMouse::LateTick(_float fTimeDelta)
{

}

HRESULT CMouse::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

void CMouse::Show_Cursor()
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CMouse::Add_Component()
{
	/* Clone은 새로운 복사본객체를 만든다. */
	/* 다만 렌더러의 경우에는 원본의 주소를 리턴하낟. Clone함수안에서 AddRef가 호출되도록. */
	/* 내 멤버변수에도 컴포넌트를 저장하고, 부모에게도 태그(키)와 함께 저장한다. */

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;	

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cursor"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMouse::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	return S_OK;
}

CMouse * CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CMouse* pInstance = new CMouse(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMouse::Clone(void* pArg)
{	
	CMouse* pInstance = new CMouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouse::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
