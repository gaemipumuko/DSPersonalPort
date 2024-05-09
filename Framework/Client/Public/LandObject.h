#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

/* 클라이언트 내에서 지형을 타야하는객체가 있다라면 지형을 태우는 연산을 하기위한 부모클래스다 .*/

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
	HRESULT SetUp_OnTerrain(class CTransform* pTransform, _float fRevisionY = 0.f /* 보정값*/ );
	// HRESULT LookAt(class CTransform* pTransform, const _float3& vTargetPos);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END