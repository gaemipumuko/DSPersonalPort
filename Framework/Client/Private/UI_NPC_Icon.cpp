#include "stdafx.h"
#include "..\Public\UI_NPC_Icon.h"

#include "GameInstance.h"

CUI_NPC_Icon::CUI_NPC_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI_NPC_Icon::CUI_NPC_Icon(const CUI_NPC_Icon & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CUI_NPC_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_NPC_Icon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eNPCType = *(NPC_ICON_TYPE*)pArg;

	switch (m_eNPCType)
	{
	case Client::CUI_NPC_Icon::NPC_ICON_BONFIRE:
		m_strObjName = TEXT("불의 계승의 제사장");
		m_iIconIndex = 0;
		break;
	case Client::CUI_NPC_Icon::NPC_ICON_SHOP:
		m_strObjName = TEXT("제사장의 시녀");
		m_iIconIndex = 5;
		break;
	case Client::CUI_NPC_Icon::NPC_ICON_BLACKSMITH:
		m_strObjName = TEXT("대장장이 안드레이");
		m_iIconIndex = 3;
		break;
	case Client::CUI_NPC_Icon::NPC_ICON_FIREKEEPER:
		m_strObjName = TEXT("화방녀");
		m_iIconIndex = 1;
		break;
	case Client::CUI_NPC_Icon::NPC_ICON_END:
		break;
	default:
		break;
	}


	//wstring strTextBox = *(wstring*)pArg;
	//lstrcpy(m_szName, strTextBox.c_str());

	m_fSizeX = 40.f;
	m_fSizeY = 40.f;

	m_fX = 117.f;
	m_fY = 88.f;
	

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CUI_NPC_Icon::Tick(_float fTimeDelta)
{
}

void CUI_NPC_Icon::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_NPC_Icon::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGameInstance->Render_Text(TEXT("Font_Default"), m_strObjName, _float2(160.f, 98.f), 0.45f, Colors::BurlyWood);

	return S_OK;
}

HRESULT CUI_NPC_Icon::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_UI_NPC_Icon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_NPC_Icon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iIconIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_NPC_Icon * CUI_NPC_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	/* 원형객체를 생성한다. */
	CUI_NPC_Icon* pInstance = new CUI_NPC_Icon(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CUI_NPC_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_NPC_Icon::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CUI_NPC_Icon* pInstance = new CUI_NPC_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_NPC_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_NPC_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
