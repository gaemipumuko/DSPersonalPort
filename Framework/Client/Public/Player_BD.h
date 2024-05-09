#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Bone.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CPlayer_BD final : public CGameObject
{
public:
	typedef struct PlayerBody_Desc
	{
		CModel*			  pParentModel;
		class CTransform* pParentTransform;
		const _float4x4*  pSocket;
		_float4x4		  PivotMatrix;
	}BODY_DESC;

private:
	CPlayer_BD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_BD(const CPlayer_BD& rhs);
	virtual ~CPlayer_BD() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;

private:

private:
	CShader*				m_pShaderCom = { nullptr };	
	CRenderer*				m_pRendererCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	

private:
	CModel*					 m_pParentModelCom = { nullptr };
	CTransform*				m_pParentTransform = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	_float4x4				m_PivotMatrix;
	_float4x4				m_WorldMatrix;

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* ������ü�� ������ �� */
	static CPlayer_BD* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �ǻ�밴ü�� ������ �� : ������ü�� ȣ���Ͽ� �纻(�ǻ�밴ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END