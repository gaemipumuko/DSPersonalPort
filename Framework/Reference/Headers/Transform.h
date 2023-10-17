#pragma once

#include "Component.h"

/* ���ÿ� �ִ� ������ ���� ��ȯ���ֱ����� ��������� �����Ѵ�. */
/* ���� ���¸� �����ϱ����� ���. */
BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) const {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float3 Get_Scaled() const {
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))), 
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))), 
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}

	_float4x4 Get_WorldFloat4x4_Inverse() {
		_float4x4	WorldMatrixInverse;
		XMStoreFloat4x4(&WorldMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
		return WorldMatrixInverse;
	}

	void Set_State(STATE eState, _fvector vState)
	{
		_matrix	TransformMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		TransformMatrix.r[eState] = vState;
		XMStoreFloat4x4(&m_WorldMatrix, TransformMatrix);
	}

	void Set_Scaled(const _float3& vScale);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

public:
	HRESULT Go_Straight(_float fTimeDelta);
	HRESULT Go_Backward(_float fTimeDelta);
	HRESULT Go_Left(_float fTimeDelta);
	HRESULT Go_Right(_float fTimeDelta);

	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);

	void Look_At(_fvector vTargetPoint);
	void Chase_Target(_fvector vTargetPoint, _float fTimeDelta, _float fMargin = 0.1f);


private:
	/* _float2, _float3, _float4, _float4x4 : �������� ������ ���� ��ȵ� */
	/* ����� ������ ����� ��������. */
	_float4x4	m_WorldMatrix;

private:
	_float		m_fSpeedPerSec = { 0.f };
	_float		m_fRotationPerSec = { 0.f };

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END