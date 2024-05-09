#include "stdafx.h"
#include "..\Public\Title_BackGround.h"
#include "Cursor.h"
#include "Sound_Manager.h"

#include "GameInstance.h"

CTitle_Background::CTitle_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CTitle_Background::CTitle_Background(const CTitle_Background & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CTitle_Background::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTitle_Background::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;

	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;

	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CTitle_Background::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_UP) & 0x80)
	{
		if (m_iCurrent_Slot == 1)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_BACKGROUND_EFFECT2);
			CSound_Manager::Get_Instance()->PlaySound(L"CURSOL_CLICK.wav", CSound_Manager::SOUND_BACKGROUND_EFFECT1, 1.f);
			m_iCurrent_Slot = 0;
		}
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		if (m_iCurrent_Slot == 0)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_BACKGROUND_EFFECT1);
			CSound_Manager::Get_Instance()->PlaySound(L"CURSOL_CLICK.wav", CSound_Manager::SOUND_BACKGROUND_EFFECT2, 1.f);
			m_iCurrent_Slot = 1;
		}
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		if (m_iCurrent_Page == 2)
		{
			m_iCurrent_Page = 1;
		}
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		if (m_iCurrent_Page == 1)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"CURSOL_CLICK.wav", CSound_Manager::SOUND_BACKGROUND_EFFECT3, 1.f);
			m_iCurrent_Page = 2;
		}
	}
}

void CTitle_Background::LateTick(_float fTimeDelta)
{
	dynamic_cast<CMouse*>(m_pGameInstance->Get_Object(LEVEL_TITLE, TEXT("Layer_UI"), TEXT("Mouse")))->Show_Cursor();


	m_pRendererCom->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CTitle_Background::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_iCurrent_Page)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTitle_Background::Add_Component()
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
	if (FAILED(__super::Add_Components(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_BackGround"),
		TEXT("Com_Texture_Background"), (CComponent**)&m_pBackgroundTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_AnyKey"),
		TEXT("Com_Texture_UI_Press_Any_Key"), (CComponent**)&m_pTextureCom[TITLE_UI_PRESS_ANY])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_Start"),
		TEXT("Com_Texture_UI_StartGame"), (CComponent**)&m_pTextureCom[TITLE_UI_STARTGAME])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_Exit"),
		TEXT("Com_Texture_UI_EndGame"), (CComponent**)&m_pTextureCom[TITLE_UI_ENDGAME])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_Select"),
		TEXT("Com_Texture_UI_Select"), (CComponent**)&m_pTextureCom[TITLE_UI_SELECT])))
		return E_FAIL;


	return S_OK;
}

HRESULT CTitle_Background::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pBackgroundTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TITLE_UI_PRESS_ANY]->Bind_ShaderResource(m_pShaderCom, "g_PressAny_Texture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TITLE_UI_STARTGAME]->Bind_ShaderResource(m_pShaderCom, "g_StartGame_Texture")))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom[TITLE_UI_ENDGAME]->Bind_ShaderResource(m_pShaderCom, "g_EndGame_Texture")))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom[TITLE_UI_SELECT]->Bind_ShaderResource(m_pShaderCom, "g_Select_Texture")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_iSlot", &m_iCurrent_Slot, sizeof(m_iCurrent_Slot))))
		return E_FAIL;


	return S_OK;
}

CTitle_Background * CTitle_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CTitle_Background* pInstance = new CTitle_Background(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTitle_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTitle_Background::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CTitle_Background* pInstance = new CTitle_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTitle_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTitle_Background::Free()
{
	__super::Free();

	Safe_Release(m_pBackgroundTextureCom);
	for (size_t i = 0; i < TITLE_UI_END; i++)
		Safe_Release(m_pTextureCom[i]);
	//Safe_Release(m_pTextureCom[TITLE_UI_PRESS_ANY]);
	//Safe_Release(m_pTextureCom[TITLE_UI_STARTGAME]);
	//Safe_Release(m_pTextureCom[TITLE_UI_ENDGAME]);
	//Safe_Release(m_pTextureCom[TITLE_UI_SELECT]);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
