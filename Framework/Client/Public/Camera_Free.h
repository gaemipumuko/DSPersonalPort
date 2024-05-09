#pragma once

#include "Client_Defines.h"
#include "Camera.h"


BEGIN(Client)

class CCamera_Free final : public CCamera
{
public:
	typedef struct  tagCameraFreeDesc
	{
		_float		fMouseSensor = 0.f;
		CCamera::CAMERA_DESC		CameraDesc;
	}CAMERA_FREE_DESC;

private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	void	SetUp_Camera(_float fCamDist = 0.f);

private:
	_float			m_fRotation = {};
	_float			m_fMouseSensor = { 0.f };
	_float			m_fCameraAngleY = {0.f};
	_float			m_fPastPlayerY = { 0.f };

	_vector			m_vCameraCentre = {};

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END