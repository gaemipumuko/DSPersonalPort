#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{

}

CComponent * CLayer::Get_Component(const wstring & strComponentTag, _uint iIndex, _int iPartObjectIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return (*iter)->Find_Component(strComponentTag, iPartObjectIndex);
}

CGameObject* CLayer::Get_Object(const wstring& strObjectName, _uint iIndex)
{
	CGameObject* pGameObject = nullptr;


	for (auto iter : m_GameObjects)
	{
		pGameObject = iter->CompareName(strObjectName);

		if (nullptr != pGameObject)
			return pGameObject;
	}

	return nullptr;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
	{
		if ((*iter)->Get_Destroy())
		{
			Safe_Release(*iter);
			iter = m_GameObjects.erase(iter);
		}
		else
		{
			(*iter)->Tick(fTimeDelta);
			++iter;
		}
	}
}

void CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->LateTick(fTimeDelta);
	}
}

CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}

