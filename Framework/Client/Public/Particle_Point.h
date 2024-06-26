#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)

class CParticle_Point final : public CGameObject
{
private:
	CParticle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Point(const CParticle_Point& rhs);
	virtual ~CParticle_Point() = default;

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
	CVIBuffer_Point_Instancing*	m_pVIBufferCom = { nullptr };
	
private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* 원형객체를 생성할 때 */
	static CParticle_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END