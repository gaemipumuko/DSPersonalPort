#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 로고씬에 필요한 객체를 만들고. 로고씬을 보여준다. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVELID eNextLevel);
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);

private:
	/* 로딩 다음레벨이 누구냐! */
	LEVELID				m_eNextLevelID = { LEVEL_END };
	class CLoader*		m_pLoader = { nullptr };

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel);
	virtual void Free() override;
};

END