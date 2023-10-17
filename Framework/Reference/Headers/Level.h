#pragma once

#include "Base.h"

/* 클라이언트 개발자가 사용하고자하는 레벨들의 부모 클래스이다. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	/* 우리는 각레벨이 시작되면 이 레벨에 필요한 객체들을 복제하여 생성한다. */
	/* 해당기능을m_pGameInstance를 통해서 바로 수행할 수 있게 될 것이다.  */
	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	virtual void Free();
};

END