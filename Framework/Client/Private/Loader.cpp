#include "stdafx.h"
#include "..\Public\Loader.h"

#pragma region UI Headers

#include "UI_Main_Menu.h"
#include "UI_Boss_Bar_Base.h"
#include "UI_Boss_Hp_Bar.h"
#include "UI_Base_Bar.h"
#include "UI_Base_Bar_L.h"
#include "UI_Base_Bar_RB.h"
#include "UI_Base_Bar_RT.h"
#include "UI_Player_Fp_Bar.h"
#include "UI_Player_Hp_Bar.h"
#include "UI_Player_Stamina_Bar.h"
#include "UI_Dialogue_BG.h"
#include "UI_NPC_Base.h"
#include "UI_NPC_Icon.h"
#include "UI_LVUP.h"
#include "Pledge_Icon.h"
#include "Interaction.h"
#include "Item_Loot.h"
#include "Inventory_Base.h"
#include "Area_Name_Center.h"
#include "ITEM_HUD_BOX.h"
#include "Souls_Indicator.h"
#include "Kuro_Obi.h"
#include "MileStone_Blurr.h"
#include "MileStone_Text.h"
#include "UI_Script_FireKeeper.h"


#pragma endregion

#pragma region Player Headers
#include "Player.h"
#include "Player_AM.h"
#include "Player_BD.h"
#include "Player_HD.h"
#include "Player_LG.h"
#include "Player_LHand.h"
#include "Player_RHand.h"

#pragma endregion


#include "Sky.h"

//#include "Effect.h"
//#include "Monster.h"
#include "Particle_Rect.h"
#include "Terrain.h"
#include "BackGround.h"
#include "Title_Background.h"
#include "Camera_Free.h"
#include "GameInstance.h"
#include "Fire_Link_Shrine.h"
#include "Fire_Link_Shrine_Pick.h"
#include "UndeadSettlement.h"
#include "Undead_Settlement_Pick.h"
#include "Map_Gundyr.h"
#include "Untended_Pick.h"
#include "Map_M40.h"
#include "FireKeeper.h"
#include "Black_Knight.h"
#include "Collider.h"
#include "Champion_Gundyr.h"
#include "Giant_Yhorm.h"

#include "Bonfire.h"

#include "Estus_Flask.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::Get_Instance())
	, m_isFinished(false)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY ThreadEntry(void* pArg)
{
	/* 새로운 스레드가 생성되었을 때, 디바이스용 컴객체를 초기화한다. */
	CoInitializeEx(nullptr, 0);

	CLoader*	pLoader = (CLoader*)pArg;

	if (FAILED(pLoader->Begin_Thread()))
		return 0;

	if (FAILED(pLoader->Loading_LevelResources()))
		return 0;

	if (FAILED(pLoader->End_Thread()))
		return 0;

	return 1;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel)
{
	m_eNextLevelID = eNextLevel;

	m_bLoaded = m_pGameInstance->Get_isLoaded();

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(0, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Begin_Thread()
{
	EnterCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::End_Thread()
{
	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::Show_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);

	return S_OK;
}

_bool CLoader::isFinished()
{
	return m_isFinished;	
}

HRESULT CLoader::Loading_LevelResources()
{
	HRESULT			hr = 0;

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_LogoLevel();
		break;
	case LEVEL_TITLE:
		hr = Loading_For_TitleLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;
	case LEVEL_FIRELINK:
		hr = Loading_For_FireLink();
		break;
	case LEVEL_GUNDYR:
		hr = Loading_For_Level_Gundyr();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_LogoLevel()
{	
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/MENU_BNE_Logo.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	//

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체 원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_BackGround*/
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
	//	CBackGround::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_TitleLevel()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/TitleLogo.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_AnyKey"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Title_UI/AnyKey.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_Select"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Title_UI/Select.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_Start"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Title_UI/Start.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TITLE, TEXT("Prototype_Component_Texture_Title_Exit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Title_UI/Exit.png")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	//

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체 원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Title_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Title_BackGround"),
		CTitle_Background::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::VertexElements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::VertexElements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::VertexElements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::VertexElements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Vtxtex_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxTex_Particle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Vtxtex_Particle.hlsl"), VTXRECT_INSTANCE::VertexElements, VTXRECT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPoint_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPoint_Particle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPoint_Particle.hlsl"), VTXPOINT_INSTANCE::VertexElements, VTXPOINT_INSTANCE::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	#pragma region GamePlay UI TEXTURES

	/* For. Prototype_Component_Texture_Area_Test */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Area_Test"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MapName1.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Souls_Indicator */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Souls_Indicator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Souls_Indicator/Souls.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Item_HUD_Box */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_HUD_Box"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/BL_HUD/ITEM_HUD_BOX/Estus_BG.png")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Pledge_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pledge_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Pledge_Icon/Pledge0.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Interaction */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Interaction"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Interaction/Interaction_Base1.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Item_Loot_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Loot_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Item_Loot.dds")))))
		return E_FAIL;


	/* For. Prototype_Component_Texture_Boss_Bar_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Boss_Bar_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss_UI/Boss_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Boss_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Boss_Hp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss_UI/Boss_HP_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Main_Menu */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Main_Menu"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainMenu.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Base_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Base_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Base_Bar_L */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Base_Bar_L"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_L.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Base_Bar_RB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Base_Bar_RB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_RB.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Base_Bar_RT */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Base_Bar_RT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_RT.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Fp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Fp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/FP_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Hp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Hp_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Stamina_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Stamina_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Stamina_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Loss_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Loss_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Loss_Bar.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Dialogue_BG */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Dialogue_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialogue/Dialogue_BG.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_NPC_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_NPC_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_Menu_Base.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_NPC_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_NPC_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_ICON%d.dds"),7))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Inventory_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Inventory_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/InvenTemp.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Inventory_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Select_Square"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/UI_Select_Square.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_Select_Tag */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Select_Tag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_Select.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Monster_Hp_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster_Hp_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Monster_UI/Monster_Hp_Base.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Estus_Flask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Estus_Flask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Items/EstusFlask/Estus_Main_%d.png"),4))))
		return E_FAIL;

#pragma endregion

	///* For. Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds")))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Brush */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png")))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	///* For. Prototype_Component_UI_Underline */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_UI_Underline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/UnderLine0.dds")))))
		return E_FAIL;

	///* For. Prototype_Component_UI_LVUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_UI_LVUP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/LVUP.dds")))))
		return E_FAIL;

	///* For. Prototype_Component_MileStone_Text */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_MileStone_Text"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MileStone/Clear.png")))))
		return E_FAIL;

	///* For. Prototype_Component_MileStone_Blurr */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_MileStone_Blurr"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MileStone/Blend.png")))))
		return E_FAIL;

	///* For. Prototype_Component_Kuro_Obi */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Kuro_Obi"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MileStone/Kuro_Obi.PNG")))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Life_Spark_4.dds"), 1))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Sky */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURECUBE, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	//	return E_FAIL;

	///* For. Prototype_Component_Texture_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;	

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */	
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
	//	return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix		PivotMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_Black_Knight */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM,"../Bin/Resources/Models/BlackKnight/Model.fbx"))))
	//	return E_FAIL;



	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tarnished"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C0000.dat", PivotMatrix))))
	//	return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Black_Knight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C1300.dat", PivotMatrix))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Gundyr"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C5110.dat", PivotMatrix))))
	//	return E_FAIL;
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Yhrom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C5260.dat", PivotMatrix))))
		return E_FAIL;


#pragma region Fire_Keeper Models


	//PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C1400.dat", PivotMatrix))))
	//	return E_FAIL;

#pragma endregion 

#pragma region Map Piece Models
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000010"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000010.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000011"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000011.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000012"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000012.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000051"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000051.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000052"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000052.fbx"))))
	//	return E_FAIL;
#pragma endregion


	/* For.Prototype_Component_Model_Bottom */
	PivotMatrix = XMMatrixScaling(0.04f, 0.04f, 0.04f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixTranslation(850.f, 343.8f, 1100.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Undead_Settlement"),
		CModel::Create(m_pDevice, m_pContext,CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C9883.dat", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Undead_Settlement_Pick */
	PivotMatrix = XMMatrixScaling(0.16f, 0.16f, 0.16f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, 1.f, 0.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Undead_Settlement_Pick"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C1240.dat", PivotMatrix))))
		return E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.04f, 0.04f, 0.04f);
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Undead_Settlement_Pick"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C1240.dat"))))
	//	return E_FAIL;

#pragma region Equipment Models
	/* For.Prototype_Component_Model_BD1950 */
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BD1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_BD1950.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_WP0206 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_WP0206"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_WP0206.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_AM1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_AM1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_AM1950.dat"))))
		return E_FAIL;
	/* For.Prototype_Component_Model_LG1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LG1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_LG1950.dat"))))
		return E_FAIL;
	/* For.Prototype_Component_Model_HD1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HD1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_HD1950.dat"))))
		return E_FAIL;

#pragma endregion

	
	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체 원형을 로딩 중 입니다."));

	///* For.Prototype_GameObject_Area_Name*/
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Area_Name"),
	//	CArea_Name_Center::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
	//	CPlayer::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Black_Knight */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Black_Knight"),
		CBlack_Knight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Fire_Link_Shrine */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Undead_Settlement"),
		CUndead_Settlement::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Undead_Settlement_Pick*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Undead_Settlement_Pick"),
		CUndead_Settlement_Pick::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Camera_Free */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
	//	CCamera_Free::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Sky */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
	//	CSky::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Yhorm_The_Giant */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yhorm_The_Giant"),
		CYhorm::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_Kuro_Obi */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Kuro_Obi"),
		CKuro_Obi::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_MileStone_Text */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MileStone_Text"),
		CMileStone_Text::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_MileStone_Blurr */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MileStone_Blurr"),
		CMileStone_Blurr::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region Game_Play UI Prototypes

	///* For.Prototype_GameObject_Item_HUD_Box */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_HUD_Box"),
	//	CItem_HUD_Box::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Pledge_Icon */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pledge_Icon"),
	//	CPledge_Icon::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Souls_Indicator */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Souls_Indicator"),
	//	CSouls_Indicator::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Item_Loot_Base */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Loot_Base"),
	//	CItem_Loot::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	//MOVED TO EACH BOSS
	    ///* For.Prototype_GameObject_Boss_Bar_Base */
	    //if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Bar_Base"),
	    //	CBoss_Bar_Base::Create(m_pDevice, m_pContext))))
	    //	return E_FAIL;
	    
	    ///* For.Prototype_GameObject_Boss_HP_Bar */
	    //if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_HP_Bar"),
	    //	CBoss_Hp_Bar::Create(m_pDevice, m_pContext))))
	    //	return E_FAIL;

	///* For.Prototype_GameObject_Main_Menu */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Main_Menu"),
	//	CUI_Main_Menu::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Base_Bar */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar"),
	//	CUI_Base_Bar::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Base_Bar_L */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar_L"),
	//	CUI_Base_Bar_L::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Base_Bar_RB */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar_RB"),
	//	CUI_Base_Bar_RB::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Base_Bar_RT */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar_RT"),
	//	CUI_Base_Bar_RT::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Player_Fp_Bar */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Fp_Bar"),
	//	CUI_Player_Fp_Bar::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Player_Hp_Bar */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Hp_Bar"),
	//	CUI_Player_Hp_Bar::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Player_Stamina_Bar */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Stamina_Bar"),
	//	CUI_Player_Stamina_Bar::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Dialogue_BG */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dialogue_BG"),
	//	CUI_Dialogue_BG::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_UI_NPC_Base */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_NPC_Base"),
	//	CUI_NPC_Base::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_UI_NPC_Icon */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_NPC_Icon"),
	//	CUI_NPC_Icon::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_UI_Inventory_Base */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Base"),
	//	CInventory_Base::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_UI_LV_UP */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LV_UP"),
	//	CUI_LVUP::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
#pragma endregion


#pragma region Player Equipments
	///* For.Prototype_GameObject_BD1950 */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BD1950"),
	//	CPlayer_BD::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_WP0206 */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_RHand"),
	//	CPlayer_RHand::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_AM1950 */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AM1950"),
	//	CPlayer_AM::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_LG1950 */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LG1950"),
	//	CPlayer_LG::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_HD1950 */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HD1950"),
	//	CPlayer_HD::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

#pragma endregion
	/* For.Prototype_GameObject_Map_M40 */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map_M40"),
	//	CMap_M40::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("콜라이더을(를) 로딩 중 입니다."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_FireLink()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::VertexElements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::VertexElements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::VertexElements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::VertexElements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Vtxtex_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Shader_VtxTex_Particle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Vtxtex_Particle.hlsl"), VTXRECT_INSTANCE::VertexElements, VTXRECT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPoint_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Shader_VtxPoint_Particle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPoint_Particle.hlsl"), VTXPOINT_INSTANCE::VertexElements, VTXPOINT_INSTANCE::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

#pragma region GamePlay UI TEXTURES

	/* For. Prototype_Component_Texture_Area_Test */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Area_Test"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MapName0.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Souls_Indicator */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Souls_Indicator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Souls_Indicator/Souls.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Item_HUD_Box */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Item_HUD_Box"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/BL_HUD/ITEM_HUD_BOX/Estus_BG.png")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Pledge_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Pledge_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Pledge_Icon/Pledge0.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Interaction */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Interaction"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Interaction/Interaction_Base1.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Item_Loot_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Item_Loot_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Item_Loot.dds")))))
		return E_FAIL;


	/* For. Prototype_Component_Texture_Boss_Bar_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Boss_Bar_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss_UI/Boss_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Boss_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Boss_Hp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss_UI/Boss_HP_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Main_Menu */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Main_Menu"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainMenu.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Base_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Base_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Base_Bar_L */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Base_Bar_L"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_L.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Base_Bar_RB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Base_Bar_RB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_RB.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Base_Bar_RT */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Base_Bar_RT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_RT.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Fp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Player_Fp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/FP_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Player_Hp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Hp_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Stamina_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Player_Stamina_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Stamina_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Loss_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Player_Loss_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Loss_Bar.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Dialogue_BG */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Dialogue_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialogue/Dialogue_BG.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_NPC_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_UI_NPC_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_Menu_Base.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_NPC_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_UI_NPC_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_ICON%d.dds"), 7))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Inventory_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Inventory_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/InvenTemp.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Inventory_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_UI_Select_Square"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/UI_Select_Square.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_Select_Tag */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_UI_Select_Tag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_Select.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Monster_Hp_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Monster_Hp_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Monster_UI/Monster_Hp_Base.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_Script_FireKeeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_UI_Script_FireKeeper"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialogue/Chat%d.png"),4))))
		return E_FAIL;

#pragma endregion

	///* For. Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	///* For. Prototype_Component_UI_Underline */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_UI_Underline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/UnderLine0.dds")))))
		return E_FAIL;

	///* For. Prototype_Component_UI_LVUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_UI_LVUP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/LVUP.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Estus_Flask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Estus_Flask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Items/EstusFlask/Estus_Main_%d.png"), 4))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Life_Spark_4.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));


	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	_matrix		PivotMatrix = XMMatrixIdentity();


#pragma region Fire_Keeper Models


	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_Fire_Keeper"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C1400.dat", PivotMatrix))))
		return E_FAIL;

#pragma endregion 


	/* For.Prototype_Component_Model_Fire_Link_Shrine */
	PivotMatrix = XMMatrixScaling(0.08f, 0.08f, 0.08f)*XMMatrixTranslation(-511.f, -58.f, 574.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_Fire_Link_Shrine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C9881.dat", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.08f, 0.08f, 0.08f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_Fire_Link_Shrine_Pick"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_M1223.dat", PivotMatrix))))
		return E_FAIL;


#pragma region Equipment Models
	/* For.Prototype_Component_Model_BD1950 */
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_BD1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_BD1950.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_WP0206 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_WP0206"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_WP0206.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_AM1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_AM1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_AM1950.dat"))))
		return E_FAIL;
	/* For.Prototype_Component_Model_LG1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_LG1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_LG1950.dat"))))
		return E_FAIL;
	/* For.Prototype_Component_Model_HD1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Model_HD1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_HD1950.dat"))))
		return E_FAIL;

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));


	if (m_bLoaded == false)
	{

	lstrcpy(m_szLoadingText, TEXT("객체 원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Area_Name*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Area_Name"),
		CArea_Name_Center::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Fire_Link_Shrine */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire_Link_Shrine"),
		CFireLinkShrine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fire_Keeper */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire_Keeper"),
		CFire_Keeper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region Game_Play UI Prototypes

	/* For.Prototype_GameObject_Item_HUD_Box */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_HUD_Box"),
		CItem_HUD_Box::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Pledge_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pledge_Icon"),
		CPledge_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Souls_Indicator */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Souls_Indicator"),
		CSouls_Indicator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Item_Loot_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Loot_Base"),
		CItem_Loot::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Main_Menu */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Main_Menu"),
		CUI_Main_Menu::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Base_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar"),
		CUI_Base_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Base_Bar_L */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar_L"),
		CUI_Base_Bar_L::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Base_Bar_RB */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar_RB"),
		CUI_Base_Bar_RB::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Base_Bar_RT */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Base_Bar_RT"),
		CUI_Base_Bar_RT::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Player_Fp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Fp_Bar"),
		CUI_Player_Fp_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Player_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Hp_Bar"),
		CUI_Player_Hp_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Player_Stamina_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Stamina_Bar"),
		CUI_Player_Stamina_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Dialogue_BG */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dialogue_BG"),
		CUI_Dialogue_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_NPC_Base */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_NPC_Base"),
	//	CUI_NPC_Base::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_UI_NPC_Icon */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_NPC_Icon"),
	//	CUI_NPC_Icon::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_UI_Inventory_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Base"),
		CInventory_Base::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_LV_UP */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LV_UP"),
		CUI_LVUP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Script_Firekeeper */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Script_Firekeeper"),
		CUI_Script_Firekeeper::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


#pragma region Player Equipments
	/* For.Prototype_GameObject_BD1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BD1950"),
		CPlayer_BD::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_WP0206 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_RHand"),
		CPlayer_RHand::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AM1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AM1950"),
		CPlayer_AM::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LG1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LG1950"),
		CPlayer_LG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HD1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HD1950"),
		CPlayer_HD::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

	/* For.Prototype_GameObject_Estus_Flask */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Estus_Flask"),
		CEstus_Flask::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bonfire*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bonfire"),
		CBonfire::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fire_Link_Shrine_Pick*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire_Link_Shrine_Pick"),
		CFireLinkShrine_Pick::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Rect"),
		CParticle_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Map_M40 */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map_M40"),
	//	CMap_M40::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	m_bLoaded = true;
	}

	lstrcpy(m_szLoadingText, TEXT("콜라이더을(를) 로딩 중 입니다."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_FIRELINK, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Gundyr()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::VertexElements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::VertexElements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::VertexElements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::VertexElements, VTXANIMMESH::iNumElements))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

#pragma region GamePlay UI TEXTURES

	/* For. Prototype_Component_Texture_Area_Test */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Area_Test"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MapName2.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Souls_Indicator */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Souls_Indicator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Souls_Indicator/Souls.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Item_HUD_Box */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Item_HUD_Box"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/BL_HUD/ITEM_HUD_BOX/Estus_BG.png")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Pledge_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Pledge_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Pledge_Icon/Pledge0.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Interaction */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Interaction"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Interaction/Interaction_Base1.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Item_Loot_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Item_Loot_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Item_Loot.dds")))))
		return E_FAIL;


	/* For. Prototype_Component_Texture_Boss_Bar_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Boss_Bar_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss_UI/Boss_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Boss_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Boss_Hp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss_UI/Boss_HP_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Main_Menu */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Main_Menu"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainMenu.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Base_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Base_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Base_Bar_L */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Base_Bar_L"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_L.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Base_Bar_RB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Base_Bar_RB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_RB.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Base_Bar_RT */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Base_Bar_RT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Base_Bar/Base_Bar_RT.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Fp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Player_Fp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/FP_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Player_Hp_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Hp_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Stamina_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Player_Stamina_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Stamina_Bar.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Player_Loss_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Player_Loss_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TL_HUD/Loss_Bar.dds")))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Dialogue_BG */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Dialogue_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialogue/Dialogue_BG.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_NPC_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_UI_NPC_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_Menu_Base.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_NPC_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_UI_NPC_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_ICON%d.dds"), 7))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Inventory_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Inventory_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/InvenTemp.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Inventory_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_UI_Select_Square"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/UI_Select_Square.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_UI_Select_Tag */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_UI_Select_Tag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/NPC_Select.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Monster_Hp_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Monster_Hp_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Monster_UI/Monster_Hp_Base.dds")))))
		return E_FAIL;

#pragma endregion

	///* For. Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds")))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Brush */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png")))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	///* For. Prototype_Component_UI_Underline */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_UI_Underline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/UnderLine0.dds")))))
		return E_FAIL;

	///* For. Prototype_Component_UI_LVUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_UI_LVUP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPC/LVUP.dds")))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Estus_Flask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Estus_Flask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Items/EstusFlask/Estus_Main_%d.png"), 4))))
		return E_FAIL;

	///* For. Prototype_Component_MileStone_Text */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_MileStone_Text"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MileStone/Clear.png")))))
		return E_FAIL;

	///* For. Prototype_Component_MileStone_Blurr */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_MileStone_Blurr"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MileStone/Blend.png")))))
		return E_FAIL;

	///* For. Prototype_Component_Kuro_Obi */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Kuro_Obi"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MileStone/Kuro_Obi.PNG")))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Life_Spark_4.dds"), 1))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Sky */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURECUBE, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	//	return E_FAIL;

	///* For. Prototype_Component_Texture_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;	

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
	//	return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix		PivotMatrix = XMMatrixIdentity();


	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_Champion_Gundyr"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C5110.dat", PivotMatrix))))
		return E_FAIL;



#pragma region Fire_Keeper Models


	//PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire_Keeper"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C1400.dat", PivotMatrix))))
	//	return E_FAIL;

#pragma endregion 

#pragma region Map Piece Models
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000010"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000010.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000011"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000011.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000012"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000012.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000051"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000051.fbx"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_m40_00_00_00_000052"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/M40/m40_00_00_00_000052.fbx"))))
	//	return E_FAIL;
#pragma endregion


	/* For.Prototype_Component_Model_Map_Gundyr */
	PivotMatrix = XMMatrixScaling(0.04f, 0.04f, 0.04f) * XMMatrixRotationX(XMConvertToRadians(90.0f));//* XMMatrixTranslation(2005.f, -619.f, -290.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_Map_Gundyr"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C1250.dat", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Map_Gundyr_Pick */
	//PivotMatrix = XMMatrixScaling(0.04f, 0.04f, 0.04f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(548.f, -31.f, -1028.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_Map_Gundyr_Pick"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C1231.dat", PivotMatrix))))
	//	return E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(1229.f, -58.f, -2958.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_Map_Gundyr_Pick"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C1231.dat", PivotMatrix))))
	//	return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixTranslation(924.f, -320.f, -120.f);;// * XMMatrixTranslation(2005.f, -619.f, -290.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_Map_Gundyr_Pick"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_C1251.dat", PivotMatrix))))
		return E_FAIL;


#pragma region Equipment Models
	/* For.Prototype_Component_Model_BD1950 */
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_BD1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_BD1950.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_WP0206 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_WP0206"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_WP0206.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_AM1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_AM1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_AM1950.dat"))))
		return E_FAIL;
	/* For.Prototype_Component_Model_LG1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_LG1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_LG1950.dat"))))
		return E_FAIL;
	/* For.Prototype_Component_Model_HD1950 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Model_HD1950"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_HD1950.dat"))))
		return E_FAIL;

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체 원형을 로딩 중 입니다."));

	///* For.Prototype_GameObject_Area_Name*/
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Area_Name"),
	//	CArea_Name_Center::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
	//	CPlayer::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_Component_Model_Map_Gundyr */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map_Gundyr"),
		CMap_Gundyr::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Map_Gundyr_Pick*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map_Gundyr_Pick"),
		CUntended_Pick::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Camera_Free */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
	//	CCamera_Free::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Sky */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
	//	CSky::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	/* For.Prototype_GameObject_Champion_Gundyr */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Champion_Gundyr"),
		CGundyr::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("콜라이더을(를) 로딩 중 입니다."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GUNDYR, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Under()
{
	return E_NOTIMPL;
}

HRESULT CLoader::Loading_For_Level_Unlit()
{
	return E_NOTIMPL;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pContext);

	Safe_Release(m_pDevice);


}
