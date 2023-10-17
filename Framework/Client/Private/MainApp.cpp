#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "Level_Loading.h"
#include "GameInstance.h"


CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
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

	if (FAILED(Ready_Gara()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component_For_Static()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw_RenderGroup();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eStartLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* 실제 사용하고자하는 레벨을 준비하기위한 레벨 로딩을 생성한다. */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevel))))
		return E_FAIL;	

	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	/* 지형 스플래팅을 위한 마스크 텍스쳐를 만든다 .*/
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

	/* ID3D11Texture2D라는 텍스쳐는 이미 생성이 되었다. */
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

	/* 생성된 텍스쳐의 정보를 활용하여 추후 텍스쳐의 데이터를 바꾸자. */	
	/* Map : pTexture2D의 공간을 걸어잠근다. 공간에 접근할 수 있도록 포인터를 리턴한다. */
	/* D3D11_MAP_WRITE_DISCARD : 실제 공간에 있는 값들은 모두 리셋된다.  */
	/* D3D11_MAP_WRITE_NO_OVERWRITE : 실제 공간에 있는 값들모두 살려둔다.  */

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

HRESULT CMainApp::Ready_Prototype_Component_For_Static()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* 모든 레벨에서 사용하는 컴포넌트들을 미리 추가해놓는다. */
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

	/* 증가되어있는 레퍼런스 카운트르 ㄹ감소시킨다. */
	Safe_Release(m_pGameInstance);

	/* 최종적으로 게임인스턴스 객체를 파괴한다. */
	CGameInstance::Release_Engine();
}
