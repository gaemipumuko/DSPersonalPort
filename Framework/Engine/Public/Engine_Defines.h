#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)

#include <d3d11.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <typeinfo>
#include <d3dcompiler.h>

#include <fx11\d3dx11effect.h>
#include <DirectXTK\DDSTextureLoader.h>
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\ScreenGrab.h>

//#include <d3d9.h>
//D3DDECLUSAGE

using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>

namespace Engine
{
	enum MOUSEKEYSTATE { MKS_LBUTTON, MKS_RBUTTON, MKS_WHEELBUTTON, MKS_XBUTTON, MKS_END };
	enum MOUSEMOVESTATE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };
}



#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"



#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace std;
using namespace Engine;