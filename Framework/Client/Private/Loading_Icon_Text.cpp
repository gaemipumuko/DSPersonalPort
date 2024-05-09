#include "stdafx.h"
#include "..\Public\Loading_Icon_Text.h"

#include "GameInstance.h"

CLoading_Icon_Text::CLoading_Icon_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CLoading_Icon_Text::CLoading_Icon_Text(const CLoading_Icon_Text & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CLoading_Icon_Text::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_Icon_Text::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;


	m_fSizeX = 90.f;
	m_fSizeY = 25.f;

	m_fX = 122;
	m_fY = 630;
	

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CLoading_Icon_Text::Tick(_float fTimeDelta)
{
	m_fFrame += fTimeDelta * 15.f;
	if (m_fFrame >= 31)
	{
		m_fFrame = 0;
	}
}

void CLoading_Icon_Text::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CLoading_Icon_Text::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Icon_Text::Add_Component()
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
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Icon_Text"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Icon_Text::Bind_ShaderResources()
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

CLoading_Icon_Text * CLoading_Icon_Text::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CLoading_Icon_Text* pInstance = new CLoading_Icon_Text(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_Icon_Text");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_Icon_Text::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CLoading_Icon_Text* pInstance = new CLoading_Icon_Text(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLoading_Icon_Text");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Icon_Text::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
