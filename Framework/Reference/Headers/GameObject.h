#pragma once

#include "Transform.h"
#include "GameInstance.h"

/* Ŭ���̾�Ʈ���� ����ϰ����ϴ� ���ӿ�����Ʈ(�÷��̾�, ���� ����) Ŭ�������� �θ� �Ǳ����� Ŭ�����̤���.  */

BEGIN(Engine)

/* 1. CTransform�� ������ ������Ʈ �Ŵ����� �߰��س����ϴ°�?! or ��ü ���� ��, Ʈ�������� ����(Ŭ��x)�Ͽ� ������Ʈ�� �ٷ� �߰��Ѵ�. */

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
	/* �ٸ� ������Ʈ�� ���� ������Ʈ�� ������Ʈ�� Ž���ϱ� �����ϵ���. */
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