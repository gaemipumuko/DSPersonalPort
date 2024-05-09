#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 로고씬에 필요한 객체를 만들고. 로고씬을 보여준다. */

BEGIN(Client)

class CLevel_Title final : public CLevel
{
private:
	CLevel_Title(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Title() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const wstring& strLayerTag);

public:
	static CLevel_Title* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:
	_bool m_bChangeLevel = { false };
	_float m_bChangeDelay = { 0.f };
};

END