#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
END

BEGIN(Client)

/* ui형태로 그리는 직교투영을 통해서 그린다. */

class CTerrain final : public CGameObject
{
public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END };
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*				m_pShaderCom = { nullptr };	
	CTexture*				m_pTextureCom[TYPE_END] = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };
	CVIBuffer_Terrain*		m_pVIBufferCom = { nullptr };
	
private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* 원형객체를 생성할 때 */
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END