#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect_Instancing;
END

BEGIN(Client)

class CParticle_Rect final : public CGameObject
{
private:
	CParticle_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Rect(const CParticle_Rect& rhs);
	virtual ~CParticle_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*					m_pShaderCom = { nullptr };	
	CTexture*					m_pTextureCom = { nullptr };
	CRenderer*					m_pRendererCom = { nullptr };
	CVIBuffer_Rect_Instancing*	m_pVIBufferCom = { nullptr };
	
private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

private:
	_float	m_fDuration = {};

public:
	/* 원형객체를 생성할 때 */
	static CParticle_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END