#pragma once

#include "Base.h"

/* 원형 객체들을 모아서 저장한다. */
/* 실 사용객체들도 모아서 저장한다. */
/* 저장해놓은 객체들의 Tick, LateTick함수를 호출해준다. */
BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex = 0);

public:
	/* 레이어들을 레벨별로 보관하기위해. 미리 레이어를 보관하는 컨테이너를 레벨만큼 할당한다. */
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject*);
	HRESULT Add_Layers(_uint iLevelIndex, const wstring& LayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void Clear(_uint LevelIndex);

private:
	map<const wstring, class CGameObject*>		m_Prototype;		

	_uint										m_iNumLevels = { 0 };
	map<const wstring, class CLayer*>*			m_pLayers = { nullptr };
	typedef map<const wstring, class CLayer*>	LAYERS;

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);

	




public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END