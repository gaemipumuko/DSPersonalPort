#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CUndead_Settlement_Pick final : public CGameObject
{
private:
	CUndead_Settlement_Pick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUndead_Settlement_Pick(const CUndead_Settlement_Pick& rhs);
	virtual ~CUndead_Settlement_Pick() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

private:



public:
	/* 원형객체를 생성할 때 */
	static CUndead_Settlement_Pick* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END