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
	_uint Get_CurrentLevelIndex() {
		return m_iCurrentLevelIndex;
	}
	_uint Get_NextLevelIndex() {
		return m_iNextLevelIndex;
	}
	_uint Get_ReservedLevelIndex() {
		return m_iReservedLevelIndex;
	}
	void Set_NextLevelIndex(_uint iLevelIndex) {
		m_iNextLevelIndex = iLevelIndex;
	}
	_bool Get_Change_Level() { return m_bChangeLevel; }
	_bool Get_isLoaded() { return m_bLoaded; }
	void Ready_Next_Level(_uint iNextLevel, _bool bLoaded);

public:
	HRESULT Initialize();
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	void Tick(_float fTimeDelta);
	HRESULT Render();
	

private:
	class CLevel*				m_pCurrentLevel = { nullptr };
	_uint						m_iCurrentLevelIndex = { 0 };
	_uint						m_iNextLevelIndex = { 0 };
	_uint						m_iReservedLevelIndex = { 0 };
	_bool						m_bChangeLevel = { false };
	_bool						m_bLoaded = { false };
public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END