#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	typedef struct tagParticleDesc
	{
		_float3		vMinPosition, vMaxPosition;
		_float2		vSpeedRange;
		_float2		vLifeTime;
		_float2		vScaleRange;
	}PARTICLE_DESC;

protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render() override;


	

protected:
	D3D11_BUFFER_DESC			m_InstanceBufferDesc = {};
	D3D11_SUBRESOURCE_DATA		m_InstanceInitialData = {};
	_uint						m_iInstanceStride = { 0 };
	_uint						m_iIndexCountPerInstance = { 0 };
	_uint						m_iNumInstance = { 0 };
	ID3D11Buffer*				m_pVBInstance = { nullptr };

protected:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

	_float*						m_pSpeeds = { nullptr };
	_float2*					m_pLifeTimes = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END