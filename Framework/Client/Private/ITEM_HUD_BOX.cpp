#include "stdafx.h"
#include "..\Public\ITEM_HUD_BOX.h"

#include "GameInstance.h"

CItem_HUD_Box::CItem_HUD_Box(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CItem_HUD_Box::CItem_HUD_Box(const CItem_HUD_Box & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CItem_HUD_Box::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_HUD_Box::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* ��׶��� ��ü�� ����ϰ����ϴ� �����ʈp�帪�� �����Ѵ�. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	BOX_SPACE	eBoxSpace = *(BOX_SPACE*)pArg;

	m_fSizeX = 70.f;
	m_fSizeY = 100.f;

	switch (eBoxSpace)
	{
	case Client::CItem_HUD_Box::BOX_LEFT:
		m_fX = 70;
		m_fY = 500;
		break;
	case Client::CItem_HUD_Box::BOX_RIGHT:
		m_fX = 230;
		m_fY = 500;
		break;
	case Client::CItem_HUD_Box::BOX_TOP:
		m_fX = 150;
		m_fY = 440;
		break;
	case Client::CItem_HUD_Box::BOX_BOTTOM:
		m_fX = 150;
		m_fY = 555;
		break;
	case Client::CItem_HUD_Box::BOX_END:
		m_fSizeX = 0.f;
		m_fSizeY = 0.f;

		m_fX = 0.f;
		m_fY = 0.f;
		break;
	default:
		break;
	}

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//���� �� �������� �ϰ������ ������ġ���� ������ �ݸ�ŭ ���ؼ�, ������ ���ݸ�ŭ ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CItem_HUD_Box::Tick(_float fTimeDelta)
{

}

void CItem_HUD_Box::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CItem_HUD_Box::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


#pragma region ��Ʈ ���

	_tchar szItemTag[MAX_PATH] = TEXT("����Ʈ ��");

	_float fLength = wcslen(szItemTag);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szItemTag), _float2(146.f, 655.f), 0.45f, Colors::White);

#pragma endregion

	return S_OK;
}

HRESULT CItem_HUD_Box::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_Item_HUD_Box"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_HUD_Box::Bind_ShaderResources()
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

CItem_HUD_Box * CItem_HUD_Box::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* ������ü�� �����Ѵ�. */
	CItem_HUD_Box* pInstance = new CItem_HUD_Box(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItem_HUD_Box");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_HUD_Box::Clone(void* pArg)
{	
	/* �ǻ�밴ü�� �����Ѵ�. ���������ȣ��. */
	CItem_HUD_Box* pInstance = new CItem_HUD_Box(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CItem_HUD_Box");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_HUD_Box::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
