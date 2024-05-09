#include "stdafx.h"
#include "..\Public\UI_Main_Menu.h"
#include "Cursor.h"
#include "GameInstance.h"

CUI_Main_Menu::CUI_Main_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI_Main_Menu::CUI_Main_Menu(const CUI_Main_Menu & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CUI_Main_Menu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Main_Menu::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	_float4 vSquare_Dimentions;

	vSquare_Dimentions.x = 24.f;
	vSquare_Dimentions.y = 19.f;
	vSquare_Dimentions.z = 62.f;
	vSquare_Dimentions.w = 62.f;

	m_pSelect_Square = CSelect_Square::Create(m_pDevice, m_pContext,&vSquare_Dimentions);


	m_fSizeX = 380.f;
	m_fSizeY = 230.f;

	m_fX = 200.f;
	m_fY = 310.f;
	

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CUI_Main_Menu::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down('O'))
	{
		m_bDestroy = true;
	}

	if (m_pGameInstance->Key_Down(VK_RETURN) && m_iMenuIndex == 0)
	{

		m_pGameInstance->Add_Layers(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI_Inventory_Base"));
		m_bDestroy = true;

	}



	if (m_pGameInstance->Key_Down(VK_UP))
	{
		if (m_iMenuIndex >= 5)
		{
			m_iMenuIndex -= 5;
		}
	}
	if (m_pGameInstance->Key_Down(VK_DOWN))
	{
		if (m_iMenuIndex <= 4)
		{
			m_iMenuIndex += 5;
		}
	}
	if (m_pGameInstance->Key_Down(VK_LEFT))
	{
		if (m_iMenuIndex > 0 && m_iMenuIndex != 5)
		{
			m_iMenuIndex--;
		}
	}
	if (m_pGameInstance->Key_Down(VK_RIGHT))
	{
		if (m_iMenuIndex < 9 && m_iMenuIndex != 4)
		{
			m_iMenuIndex++;
		}
	}

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	switch (m_iMenuIndex)
	{
		/*
0 = 24 19
1 = 88 19
2 = 154 19
3 = 220 19
4 = 285 19
5 = 25 118
6 = 90 118
7 = 155 118
8 = 220 118
9 = 285 118
*/
	case 0:
		m_pSelect_Square->Set_Position(m_fX + 24.f, m_fY +19.f);
		break;
	case 1:
		m_pSelect_Square->Set_Position(m_fX + 88.f, m_fY + 19.f);
		break;
	case 2:
		m_pSelect_Square->Set_Position(m_fX + 154.f, m_fY + 19.f);
		break;
	case 3:
		m_pSelect_Square->Set_Position(m_fX + 220.f, m_fY + 19.f);
		break;
	case 4:
		m_pSelect_Square->Set_Position(m_fX + 285.f, m_fY + 19.f);
		break; 
	case 5:
		m_pSelect_Square->Set_Position(m_fX + 25.f, m_fY + 118.f);
		break;
	case 6:
		m_pSelect_Square->Set_Position(m_fX + 90.f, m_fY + 118.f);
		break;
	case 7:
		m_pSelect_Square->Set_Position(m_fX + 155.f, m_fY + 118.f);
		break;
	case 8:
		m_pSelect_Square->Set_Position(m_fX + 220.f, m_fY + 118.f);
		break; 
	case 9:
		m_pSelect_Square->Set_Position(m_fX + 285.f, m_fY + 118.f);
		break;
	default:
		m_iMenuIndex = 0;
		break;

	}

	m_pSelect_Square->Tick(fTimeDelta);

}

void CUI_Main_Menu::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);

	dynamic_cast<CMouse*>(m_pGameInstance->Get_Object(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_UI"), TEXT("Mouse")))->Show_Cursor();

	m_pSelect_Square->LateTick(fTimeDelta);
}

HRESULT CUI_Main_Menu::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pSelect_Square->Render();

	return S_OK;
}

HRESULT CUI_Main_Menu::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_Main_Menu"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Main_Menu::Bind_ShaderResources()
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

CUI_Main_Menu * CUI_Main_Menu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CUI_Main_Menu* pInstance = new CUI_Main_Menu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Main_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Main_Menu::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CUI_Main_Menu* pInstance = new CUI_Main_Menu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Main_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Main_Menu::Free()
{
	__super::Free();
	Safe_Release(m_pSelect_Square);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
