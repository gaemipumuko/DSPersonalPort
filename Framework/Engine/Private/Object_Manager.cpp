#include "..\Public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

CObject_Manager::CObject_Manager()
{
}

CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strComponentTag, _uint iIndex, _int iPartObjectIndex)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex, iPartObjectIndex);
	
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

/* 원형객체를 추가한다. */
HRESULT CObject_Manager::Add_Prototype(const wstring& strPrototypeTag, CGameObject * pPrototype)
{
	/* 이미 같은 태그로 원형ㅇ르 추가했었어. */
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototype.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

CGameObject* CObject_Manager::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}


/* 원형객체를 찾아서 복제하여 실사용하기위한 객체를 레이어에 추가한다. */
HRESULT CObject_Manager::Add_Layers(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void * pArg)
{
	CGameObject*		pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 우ㅕㄴ형객체를 보겟ㅎ하여 새로운 객체를 새엇ㅇ했더ㅏ /.*/
	CGameObject*		pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	/* 생성한 새로운객체를 추가할 레이어를 검색한다. */
	CLayer*			pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/* 추가할 레이어가 없다?! */
	if (nullptr == pLayer)
	{
		/* 만들어야지 .*/
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)	
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(fTimeDelta);
		}
	}
	
}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->LateTick(fTimeDelta);
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	/* 로고레벨에 추가되어있었던 백그라운드레이어, 플레이어레이어, 유아이레이어. */
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{	
	auto	iter = m_Prototype.find(strPrototypeTag);

	if (iter == m_Prototype.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring & strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CGameObject* CObject_Manager::Get_Object(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strObjectName, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Object(strObjectName, iIndex);
}

_uint CObject_Manager::Get_NumObjects(_uint iLevelIndex, const wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return 0;


	return pLayer->Get_NumObjects();
}

CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager* pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	/* 사본ㄴ객체 정리. */
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}		
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

	/* 원본객체 정리. */
	for (auto& Pair : m_Prototype)
	{
		Safe_Release(Pair.second);
	}
	m_Prototype.clear();
}
