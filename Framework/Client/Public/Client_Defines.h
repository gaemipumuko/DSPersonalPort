#pragma once

#include <process.h>
#include <io.h>

namespace Client
{
	const static unsigned int		g_iWinSizeX = 1280;
	const static unsigned int		g_iWinSizeY = 720;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_TITLE, LEVEL_GAMEPLAY,LEVEL_FIRELINK,LEVEL_GUNDYR,LEVEL_UNDER,LEVEL_UNLIT, LEVEL_END };

	typedef struct Player_Params
	{
		float		fMaxHp;
		float		fCurrentHp;
		float		fMaxStamina;
		float		fCurrentStamina;
		float		fMaxFp;
		float		fCurrentFp;
		float		fRigidity;
		float		fFind;
		float		fCurrentWeight;
		float		fMaxWeight;

		unsigned int		iLevel;
		unsigned int		iVigor;
		unsigned int		iAttunement;
		unsigned int		iEndurance;
		unsigned int		iVitality;
		unsigned int		iStrength;
		unsigned int		iDexterity;
		unsigned int		iIntelligence;
		unsigned int		iFaith;
		unsigned int		iLuck;
		unsigned int		iMemorySlots;
		unsigned int		iSouls;

	}PLAYER_PARAM;

	typedef struct Non_Player_Params
	{
		float		fMaxHp;
		float		fCurrentHp;
		float		fRigidity;

		unsigned int		iSouls;

	}NPC_PARAM;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

using namespace Client;
