#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

CLevel_Manager::CLevel_Manager()
{

}

HRESULT CLevel_Manager::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if(nullptr != m_pCurrentLevel)
	pGameInstance->Clear(m_iCurrentLevelIndex);

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;

	m_iCurrentLevelIndex = iLevelIndex;

	m_bChangeLevel = false;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();	
}

void CLevel_Manager::Ready_Next_Level(_uint iNextLevel, _bool bLoaded)
{
	m_bChangeLevel = true;
	m_iReservedLevelIndex = iNextLevel;
	m_bLoaded = bLoaded;
}

CLevel_Manager * CLevel_Manager::Create()
{
	CLevel_Manager* pInstance = new CLevel_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
