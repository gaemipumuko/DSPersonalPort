#include "stdafx.h"
#include "..\Public\Level_Loading.h"


#include "Level_Logo.h"
#include "Level_GamePlay.h"

#include "Loader.h"
#include "GameInstance.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevelID = eNextLevel;

	/* ���� ������ �ʿ��� �ڿ��� �غ��ϱ� ���ؼ�. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pLoader->Show_LoadingText();

	///* �� �ε� ������?! */
	if (true == m_pLoader->isFinished())
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			CLevel*		pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;		

			if (FAILED(m_pGameInstance->Open_Level(m_eNextLevelID, pLevel)))
			{
				MSG_BOX("Failed to Created : New Level");
				return;
			}

			/* ���� ���� �Ҵ翡 �����Ͽ���. */
			/* �̹� �ε� ������ �ı��Ǿ��� ���̴�. */
			/* �ε� ������ �Լ��� ����ϴµ� ������ ������ ������ �ε� ������ ��� ������ ����ϰ� �Ǹ� ���� ������. */
			return;
		}
	}	
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
