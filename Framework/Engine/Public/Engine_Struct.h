#pragma once

namespace Engine
{
	typedef struct tagGraphic_Desc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND			hWnd;
		unsigned int	iWinSizeX, iWinSizeY;
		WINMODE			isWindowed;

	}GRAPHIC_DESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END};

		TYPE		eLightType = TYPE_END;
		XMFLOAT4	vDirection;
		XMFLOAT4	vPosition;
		float		fRange;

		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;		
	}LIGHT_DESC;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;		

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Cube_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXCUBETEX;

	

	
}

