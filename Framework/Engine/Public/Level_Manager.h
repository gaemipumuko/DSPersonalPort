#pragma once

#include "Base.h"


/* 현재 할당된 레벨을 보관한다. */
/* 레벨 교체 시, 기존레벨용 자원들을 지우고 기존 레벨을 삭제하고 새로운 레벨로 저장한다. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	void Tick(_float fTimeDelta);
	HRESULT Render();
	

private:
	class CLevel*				m_pCurrentLevel = { nullptr };
	_uint						m_iCurrentLevelIndex = { 0 };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END