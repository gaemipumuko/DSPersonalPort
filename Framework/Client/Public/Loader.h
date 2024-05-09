#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 지정된 레벨에 필요한 준비를 한다. */

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:	
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;


public:
	HRESULT Initialize(LEVELID eNextLevel);
	HRESULT Begin_Thread();
	HRESULT End_Thread();
	HRESULT Show_LoadingText();
	_bool isFinished();
	void	Set_Loaded(_bool bLoaded) { m_bLoaded = bLoaded; }

public:
	HRESULT Loading_LevelResources();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };
	LEVELID						m_eNextLevelID = { LEVEL_END };
	_tchar						m_szLoadingText[MAX_PATH] = TEXT("");
	_bool						m_isFinished = { false };
	_bool						m_bLoaded = { false };

private:
	HANDLE						m_hThread = { 0 };
	CRITICAL_SECTION			m_Critical_Section;



private:
	HRESULT Loading_For_LogoLevel();
	HRESULT Loading_For_TitleLevel();
	HRESULT Loading_For_GamePlayLevel();
	HRESULT Loading_For_FireLink();
	HRESULT Loading_For_Level_Gundyr();
	HRESULT Loading_For_Level_Under();
	HRESULT Loading_For_Level_Unlit();

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel);
	virtual void Free() override;

};

END