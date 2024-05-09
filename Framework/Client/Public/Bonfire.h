#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Interaction.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CBonfire final : public CGameObject
{
private:
	CBonfire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBonfire(const CBonfire& rhs);
	virtual ~CBonfire() = default;

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

	CInteraction* m_pInteraction_Box = { nullptr };

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

private:
	_bool m_bIsApproched = {true};
	_bool m_bChangeMap = { false };

	_float m_fChangeDelay = {0.f};

public:
	/* 원형객체를 생성할 때 */
	static CBonfire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END