#pragma once

#include "Base.h"

/* Ŭ���̾�Ʈ �����ڰ� ����ϰ����ϴ� �������� �θ� Ŭ�����̴�. */

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

	/* �츮�� �������� ���۵Ǹ� �� ������ �ʿ��� ��ü���� �����Ͽ� �����Ѵ�. */
	/* �ش�����m_pGameInstance�� ���ؼ� �ٷ� ������ �� �ְ� �� ���̴�.  */
	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	virtual void Free();
};

END