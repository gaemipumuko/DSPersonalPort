#include "stdafx.h"
#include "..\Public\Level_M40.h"
#include "Camera_Free.h"
#include "ITEM_HUD_BOX.h"

CLevel_M40::CLevel_M40(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_M40::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_M40::Tick(_float fTimeDelta)
{

	if (m_bCursorLocked == true)
	{
	SetCursorPos(g_iWinSizeX / 2, g_iWinSizeY / 2);
	}


	if (m_pGameInstance->Key_Down('O'))
	{
		if(m_bMenuOpen == false)
		{
		m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_Main_Menu"));
		m_bMenuOpen = true;
		}
		else
		{
			m_bMenuOpen = false;
		}

	}

	if (m_pGameInstance->Key_Down('U'))
	{
		m_bCursorLocked = !m_bCursorLocked;
	}

	__super::Tick(fTimeDelta);
	

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
}

HRESULT CLevel_M40::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_M40::Ready_Lights()
{
	LIGHT_DESC			LightDesc = {};

	LightDesc.eLightType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LEVEL_GAMEPLAY, LightDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_M40::Ready_Layer_Camera(const wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC			CameraFreeDesc = {  };

	CameraFreeDesc.fMouseSensor = 0.1f;
	CameraFreeDesc.CameraDesc.vEye = _float4(0.f, 10.f, -15.f, 1.f);
	CameraFreeDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraFreeDesc.CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraFreeDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraFreeDesc.CameraDesc.fNear = 0.2f;
	CameraFreeDesc.CameraDesc.fFar = 1000.0f;
	CameraFreeDesc.CameraDesc.GameObjectDesc.TransformDesc.fSpeedPerSec = 50.f;
	CameraFreeDesc.CameraDesc.GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraFreeDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_M40::Ready_Layer_BackGround(const wstring & strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Fire_Link_Shrine"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_M40::Ready_Layer_Player(const wstring & strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	m_pCamera->SetUp_Camera();


	return S_OK;
}

HRESULT CLevel_M40::Ready_Layer_Monster(const wstring & strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Fire_Keeper"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_M40::Ready_Layer_Effect(const wstring & strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_M40::Ready_Layer_UI(const wstring& strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Cursor"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Area_Name"))))
		return E_FAIL;

	CItem_HUD_Box::BOX_SPACE eBoxSpace;

	eBoxSpace = CItem_HUD_Box::BOX_LEFT;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Item_HUD_Box"),&eBoxSpace)))
		return E_FAIL;
	eBoxSpace = CItem_HUD_Box::BOX_RIGHT;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Item_HUD_Box"),&eBoxSpace)))
		return E_FAIL;
	eBoxSpace = CItem_HUD_Box::BOX_TOP;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Item_HUD_Box"),&eBoxSpace)))
		return E_FAIL;
	eBoxSpace = CItem_HUD_Box::BOX_BOTTOM;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Item_HUD_Box"),&eBoxSpace)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Pledge_Icon"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Souls_Indicator"))))
		return E_FAIL;

	_float2 fUIPos;
	fUIPos.x = 130.f;
	fUIPos.y = 50.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar"),&fUIPos)))
		return E_FAIL;
	fUIPos.x = 130.f;
	fUIPos.y = 61.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar"), &fUIPos)))
		return E_FAIL;
	fUIPos.x = 130.f;
	fUIPos.y = 72.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar"), &fUIPos)))
		return E_FAIL;

	fUIPos.x = 127.f;
	fUIPos.y = 49.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar_L"), &fUIPos)))
		return E_FAIL;
	fUIPos.x = 127.f;
	fUIPos.y = 59.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar_L"), &fUIPos)))
		return E_FAIL;

	fUIPos.x = 127.f;
	fUIPos.y = 60.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar_L"), &fUIPos)))
		return E_FAIL;
	fUIPos.x = 127.f;
	fUIPos.y = 70.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar_L"), &fUIPos)))
		return E_FAIL;
	fUIPos.x = 127.f;
	fUIPos.y = 71.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar_L"), &fUIPos)))
		return E_FAIL;
	fUIPos.x = 127.f;
	fUIPos.y = 80.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Base_Bar_L"), &fUIPos)))
		return E_FAIL;

	fUIPos.x = 130.f;
	fUIPos.y = 52.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player_Hp_Bar"), &fUIPos)))
		return E_FAIL;

	fUIPos.x = 130.f;
	fUIPos.y = 63.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player_Fp_Bar"), &fUIPos)))
		return E_FAIL;

	fUIPos.x = 130.f;
	fUIPos.y = 74.f;
	if (FAILED(m_pGameInstance->Add_Layers(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player_Stamina_Bar"), &fUIPos)))
		return E_FAIL;

	return S_OK;
}

CLevel_M40 * CLevel_M40::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_M40* pInstance = new CLevel_M40(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_M40");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_M40::Free()
{
	__super::Free();
}
