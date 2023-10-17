#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{

		_float4		vEye, vAt;
		_float		fFovy, fAspect, fNear, fFar;

		CGameObject::GAMEOBJECT_DESC	GameObjectDesc;

	}CAMERA_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

private:
	_float				m_fFovy = { 0.0f };
	_float				m_fAspect = { 0.0f };
	_float				m_fNear = { 0.0f };
	_float				m_fFar = { 0.0f };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END