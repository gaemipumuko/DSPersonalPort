#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

/* Ŭ���̾�Ʈ ������ ������ Ÿ���ϴ°�ü�� �ִٶ�� ������ �¿�� ������ �ϱ����� �θ�Ŭ������ .*/

BEGIN(Engine)
class CTransform;
END


BEGIN(Client)

class CLandObject abstract : public CGameObject
{
protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT SetUp_OnTerrain(class CTransform* pTransform, _float fRevisionY = 0.f /* ������*/ );
	// HRESULT LookAt(class CTransform* pTransform, const _float3& vTargetPos);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END