#pragma once

#include "Base.h"

/* ���� ��ü���� ��Ƽ� �����Ѵ�. */
/* �� ��밴ü�鵵 ��Ƽ� �����Ѵ�. */
/* �����س��� ��ü���� Tick, LateTick�Լ��� ȣ�����ش�. */
BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex , _int iPartObjectIndex);
	class CGameObject* Get_Object(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strObjectName, _uint iIndex = 0);
	_uint Get_NumObjects(_uint iLevelIndex, const wstring& strLayerTag);

public:
	/* ���̾���� �������� �����ϱ�����. �̸� ���̾ �����ϴ� �����̳ʸ� ������ŭ �Ҵ��Ѵ�. */
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject*);
	class CGameObject* Clone_GameObject(const wstring& strPrototypeTag, void* pArg);
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