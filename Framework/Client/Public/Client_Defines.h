#pragma once

#include <process.h>

namespace Client
{
	const static unsigned int		g_iWinSizeX = 1280;
	const static unsigned int		g_iWinSizeY = 720;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

using namespace Client;
