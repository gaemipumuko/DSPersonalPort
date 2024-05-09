#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Bone.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

class CPlayer_RHand final : public CGameObject
{
public:
	typedef struct Player_RHand_Desc
	{
		class CTransform*   pParentTransform;
		const _float4x4*	pSocket;
		_float4x4			PivotMatrix;
	}HAND_DESC_R;

private:
	CPlayer_RHand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_RHand(const CPlayer_RHand& rhs);
	virtual ~CPlayer_RHand() = default;

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
	CCollider*				m_pColliderCom = { nullptr };

private:
	CTransform*				m_pParentTransform = { nullptr };
	const _float4x4*		m_pSocketMatrix = { nullptr };
	_float4x4				m_PivotMatrix;
	_float4x4				m_WorldMatrix;

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* 원형객체를 생성할 때 */
	static CPlayer_RHand* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END