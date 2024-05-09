#include "stdafx.h"
#include "..\Public\UI_Boss_Bar_Base.h"

#include "GameInstance.h"

CBoss_Bar_Base::CBoss_Bar_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBoss_Bar_Base::CBoss_Bar_Base(const CBoss_Bar_Base & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CBoss_Bar_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Bar_Base::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	wstring strTextBox = *(wstring*)pArg;
	lstrcpy(m_szName, strTextBox.c_str());


	m_fSizeX = 670.f;
	m_fSizeY = 15.f;

	m_fX = 370.f;
	m_fY = 595.f;
	

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CBoss_Bar_Base::Tick(_float fTimeDelta)
{
	m_fRenderTime -= fTimeDelta;

	if (m_fRenderTime < 0)
	{
		m_iDamageTaken = 0;
	}
}

void CBoss_Bar_Base::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CBoss_Bar_Base::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


#pragma region 이름 출력

	wstring szDamage;

	szDamage = to_wstring(m_iDamageTaken);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(m_szName), _float2(370.f, 570.f), 0.5f, Colors::White);

	if(m_iDamageTaken != 0 )
	m_pGameInstance->Render_Text(TEXT("Font_Default"), szDamage, _float2(995.f, 575.f), 0.4f, Colors::White);


#pragma endregion

	return S_OK;
}

void CBoss_Bar_Base::Update(_uint iDamage)
{
	m_fRenderTime = 2.f;

	m_iDamageTaken += iDamage;
}

HRESULT CBoss_Bar_Base::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_Boss_Bar_Base"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Bar_Base::Bind_ShaderResources()
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

CBoss_Bar_Base * CBoss_Bar_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	/* 원형객체를 생성한다. */
	CBoss_Bar_Base* pInstance = new CBoss_Bar_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_Bar_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Bar_Base::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CBoss_Bar_Base* pInstance = new CBoss_Bar_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_Bar_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Bar_Base::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
