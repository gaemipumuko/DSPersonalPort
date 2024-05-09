#include "stdafx.h"
#include "..\Public\Loading_Icon.h"

#include "GameInstance.h"

CLoading_Icon::CLoading_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CLoading_Icon::CLoading_Icon(const CLoading_Icon & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CLoading_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_Icon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* ��׶��� ��ü�� ����ϰ����ϴ� �����ʈp�帪�� �����Ѵ�. */
	if (FAILED(Add_Component()))
		return E_FAIL;


	m_fSizeX = 85.f;
	m_fSizeY = 85.f;

	m_fX = 125;
	m_fY = 565;
	

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//���� �� �������� �ϰ������ ������ġ���� ������ �ݸ�ŭ ���ؼ�, ������ ���ݸ�ŭ ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CLoading_Icon::Tick(_float fTimeDelta)
{
	m_fFrame += fTimeDelta * 15.f;
	if (m_fFrame >= 31)
	{
		m_fFrame = 0;
	}
}

void CLoading_Icon::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CLoading_Icon::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 0.f, 1.f));

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Icon::Add_Component()
{
	/* Clone�� ���ο� ���纻��ü�� �����. */
	/* �ٸ� �������� ��쿡�� ������ �ּҸ� �����ϳ�. Clone�Լ��ȿ��� AddRef�� ȣ��ǵ���. */
	/* �� ����������� ������Ʈ�� �����ϰ�, �θ𿡰Ե� �±�(Ű)�� �Բ� �����Ѵ�. */

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
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Icon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Icon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", (_uint)m_fFrame)))
		return E_FAIL;

	return S_OK;
}

CLoading_Icon * CLoading_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* ������ü�� �����Ѵ�. */
	CLoading_Icon* pInstance = new CLoading_Icon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_Icon::Clone(void* pArg)
{	
	/* �ǻ�밴ü�� �����Ѵ�. ���������ȣ��. */
	CLoading_Icon* pInstance = new CLoading_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLoading_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
