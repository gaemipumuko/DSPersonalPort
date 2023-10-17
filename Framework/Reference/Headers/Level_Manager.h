#pragma once

#include "Base.h"


/* ���� �Ҵ�� ������ �����Ѵ�. */
/* ���� ��ü ��, ���������� �ڿ����� ����� ���� ������ �����ϰ� ���ο� ������ �����Ѵ�. */

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