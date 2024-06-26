#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 게임플레이씬에 필요한 객체를 만들고. 게임프렐이씬을 보여준다. */

BEGIN(Client)

class CLevel_FireLink final : public CLevel
{
private:
	CLevel_FireLink(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_FireLink() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const wstring& strLayerTag);

	HRESULT Ready_Level_Change(LEVELID eNextLevel);
private:

	_bool			m_bMenuOpen = { false };
	_bool			m_bNPCMenuOpen = { false };
	_bool			m_bInvenOpen = { false };
	_bool			m_bCursorLocked = { true };

	class CCamera_Free* m_pCamera = { nullptr };
public:
	static CLevel_FireLink* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END