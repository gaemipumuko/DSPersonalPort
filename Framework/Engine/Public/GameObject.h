#pragma once

#include "Transform.h"
#include "GameInstance.h"

/* 클라이언트에서 사용하고자하는 게임오브젝트(플레이어, 몬스터 등등등) 클래스들의 부모가 되기위한 클래스이ㅏㄷ.  */

BEGIN(Engine)

/* 1. CTransform의 원형을 컴포넌트 매니져에 추가해놔야하는가?! or 객체 생성 시, 트랜스폼을 생성(클론x)하여 컴포넌트로 바로 추가한다. */

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc
	{		
		tagGameObjectDesc(_float _fSpeedPerSec = 0.f, _float _fRotationPerSec = 0.f, const wstring& _strTransformTag = TEXT(""))
			: strTransformTag(_strTransformTag)
			, TransformDesc { _fSpeedPerSec, XMConvertToRadians(_fRotationPerSec) } { }
		wstring						strTransformTag = TEXT("");
		CTransform::TRANSFORM_DESC	TransformDesc;
	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	class CComponent* Find_Component(const wstring& strComTag);

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

protected:
	wstring						m_strTransformTag = TEXT("Com_Transform");
	class CTransform*			m_pTransformCom = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
protected:
	/* 다른 오브젝트가 현재 오브젝트의 컴포넌트를 탐색하기 용이하도록. */
	map<const wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Components(_uint iLevelIndex, 
		const wstring& strPrototypeTag, 
		const wstring& strComTag, 
		_Inout_ CComponent** ppComponent, 
		void* pArg = nullptr);



public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END