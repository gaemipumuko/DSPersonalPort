#include "stdafx.h"
#include "..\Public\Level_Title.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Sound_Manager.h"

CLevel_Title::CLevel_Title(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Title::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CSound_Manager::Get_Instance()->StopAll();
	CSound_Manager::Get_Instance()->PlayBGM(L"Title.wav", 0.5f);

	return S_OK;
}

void CLevel_Title::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_BACKGROUND_EFFECT3);
		CSound_Manager::Get_Instance()->PlaySound(L"GAMESTART.wav", CSound_Manager::SOUND_BACKGROUND_EFFECT3, 0.6f);

		m_bChangeLevel = true;
	}

	if (m_bChangeLevel == true)
	{
		m_bChangeDelay += fTimeDelta;
		if (m_bChangeDelay >= 0.7f)
		{
			if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_FIRELINK))))
			{
				MSG_BOX("Failed to Created : Level_Loading");
				return;
			}
		}
	}

	SetWindowText(g_hWnd, TEXT("타이틀레벨입니다."));
}

HRESULT CLevel_Title::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Title::Ready_Layer_BackGround(const wstring & strLayerTag)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_TITLE, strLayerTag, TEXT("Prototype_GameObject_Title_BackGround"))))
		return E_FAIL;			
	
	return S_OK;
}

HRESULT CLevel_Title::Ready_Layer_UI(const wstring& strLayerTag)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_TITLE, strLayerTag, TEXT("Prototype_GameObject_Cursor"))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Layers(LEVEL_TITLE, strLayerTag, TEXT("Prototype_GameObject_Title_BackGround"))))
	//	return E_FAIL;

	return S_OK;
}



CLevel_Title * CLevel_Title::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Title* pInstance = new CLevel_Title(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Title");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Title::Free()
{
	__super::Free();
}
