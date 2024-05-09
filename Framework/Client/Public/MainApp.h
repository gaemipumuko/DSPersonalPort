#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	// IDirect3DDevice9 = ID3D11Device + ID3D11DeviceContext
	ID3D11Device*			m_pDevice = { nullptr }; /* 공간할당(정점버퍼, 인덱스버퍼, 텍스쳐 등등을 생성한다) */
	ID3D11DeviceContext*	m_pContext = { nullptr }; /* 기능(그린다. 장치에 바인딩한다) */

private:
	CGameInstance*			m_pGameInstance = { nullptr };
	CRenderer*				m_pRenderer = { nullptr };
	class CSound_Manager* m_pSound_Manager = { nullptr };

#ifdef _DEBUG
private:
	_float					m_fTimeAcc = { 0.0f };
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_uint					m_iNumRender = { 0 };
#endif

private:
	HRESULT Open_Level(LEVELID eStartLevel);
	HRESULT Ready_Gara();
	HRESULT Ready_Fonts();
	HRESULT Ready_Prototype_Component_For_Static();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END