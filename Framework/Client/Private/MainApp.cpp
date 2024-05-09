#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "Sound_Manager.h"
#include "BackGround.h"
#include "Loading_Icon.h"
#include "Loading_Info.h"
#include "Loading_Icon_Text.h"
#include "Cursor.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
	,m_pSound_Manager(CSound_Manager::Get_Instance())
{	
	// D3D11_SAMPLER_DESC 

	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	GRAPHIC_DESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.isWindowed = GRAPHIC_DESC::MODE_WIN;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;	

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInstance, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	//D3D11_BLEND_DESC		BlendDesc;
	//D3D11_DEPTH_STENCIL_DESC;
	//D3D11_RASTERIZER_DESC;

	//ID3D11BlendState*		pBlendState;
	//ID3D11DepthStencilState*;
	//ID3D11RasterizerState*


	//m_pDevice->CreateBlendState(BlendDesc, pBlendState);


	//m_pContext->OMSetBlendState(pBlendState);
	//m_pContext->OMSetDepthStencilState();
	//m_pContext->RSSetState();

	//if (FAILED(Ready_Gara()))
	//	return E_FAIL;

	if (FAILED(Ready_Prototype_Component_For_Static()))
		return E_FAIL;

	if (FAILED(Ready_Fonts()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	ShowCursor(FALSE);

	if (nullptr == m_pGameInstance)
		return;

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif
	 
	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(1.f, 1.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw_RenderGroup();

#ifdef _DEBUG	
	++m_iNumRender;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("��� FPS : %d"), m_iNumRender);
		m_fTimeAcc = 0;
		m_iNumRender = 0;
	}

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(m_szFPS), _float2(0.f, 0.f), 0.5f, Colors::BurlyWood);

#endif

	m_pGameInstance->Present();

	if (m_pGameInstance->Change_Level())
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVELID>(m_pGameInstance->Get_Reserved_Level_Index())))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eStartLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* ���� ����ϰ����ϴ� ������ �غ��ϱ����� ���� �ε��� �����Ѵ�. */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevel))))
		return E_FAIL;	

	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	/* ���� ���÷����� ���� ����ũ �ؽ��ĸ� ����� .*/
	ID3D11Texture2D*		pTexture2D = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc = {};
	TextureDesc.Width = 128;
	TextureDesc.Height = 128;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;		
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_ulong*			pPixels = new _ulong[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pPixels, sizeof(_ulong) * TextureDesc.Width * TextureDesc.Height);




	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			pPixels[iIndex] = D3DCOLOR_ABGR(255, 255, 255, 255);
		}
	}
	
	D3D11_SUBRESOURCE_DATA		InitialData = {};
	InitialData.pSysMem = pPixels;
	InitialData.SysMemPitch = TextureDesc.Width * 4;	

	/* ID3D11Texture2D��� �ؽ��Ĵ� �̹� ������ �Ǿ���. */
	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;
	
	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			if(j < 64)
				pPixels[iIndex] = D3DCOLOR_ABGR(255, 255, 255, 255);
			else
				pPixels[iIndex] = D3DCOLOR_ABGR(255, 0, 0, 0);
		}
	}

	/* ������ �ؽ����� ������ Ȱ���Ͽ� ���� �ؽ����� �����͸� �ٲ���. */	
	/* Map : pTexture2D�� ������ �ɾ���ٴ�. ������ ������ �� �ֵ��� �����͸� �����Ѵ�. */
	/* D3D11_MAP_WRITE_DISCARD : ���� ������ �ִ� ������ ��� ���µȴ�.  */
	/* D3D11_MAP_WRITE_NO_OVERWRITE : ���� ������ �ִ� ������ ����д�.  */

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	memcpy(SubResource.pData, pPixels, sizeof(_ulong) * TextureDesc.Width * TextureDesc.Height);

	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(DirectX::SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"))))
		return E_FAIL;


	Safe_Delete_Array(pPixels);
	Safe_Release(pTexture2D);

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	// MakeSpriteFont "�ؽ�lv1��� Bold" /FontSize:30 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 138ex.spritefont
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Default"), TEXT("../Bin/Resources/Fonts/138ex.spriteFont"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_For_Static()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Cursor.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/MENU_BNE_Logo.dds")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Item_Info"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Loading/Item_Info/exLoading%d.jpg"),7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Loading/output/Loading_Icon%d.png"),31))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Icon_Text"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Loading/LoadingIcon_LOGO.dds")))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cursor"),
		CMouse::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Loading_Icon"),
		CLoading_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Loading_Icon_Text"),
		CLoading_Icon_Text::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Loading_Info"),
		CLoading_Info::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* ��� �������� ����ϴ� ������Ʈ���� �̸� �߰��س��´�. */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_Shader_VtxTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXPOSTEX::VertexElements, VTXPOSTEX::iNumElements))))
		return E_FAIL;	


	/* For.Prototype_Component_VIBuffer_Rect_Instancing */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Instancing"),
		CVIBuffer_Rect_Instancing::Create(m_pDevice, m_pContext, 200))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Vtxtex_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex_Particle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Vtxtex_Particle.hlsl"), VTXRECT_INSTANCE::VertexElements, VTXRECT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPoint_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPoint_Particle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPoint_Particle.hlsl"), VTXPOINT_INSTANCE::VertexElements, VTXPOINT_INSTANCE::iNumElements))))
		return E_FAIL;


	_matrix		PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Tarnished"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/DataFiles/Loaded/Model_C0000.dat", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.06f, 0.06f, 0.06f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bonfire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/DataFiles/Loaded/Model_obj0000.dat", PivotMatrix))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSound_Manager);

	/* �����Ǿ��ִ� ���۷��� ī��Ʈ�� �����ҽ�Ų��. */
	Safe_Release(m_pGameInstance);

	/* ���������� �����ν��Ͻ� ��ü�� �ı��Ѵ�. */
	CGameInstance::Release_Engine();
}
