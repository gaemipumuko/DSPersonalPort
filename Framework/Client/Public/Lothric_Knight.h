#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CChannel;
END

BEGIN(Client)

class CLothric_Knight final : public CGameObject
{
	enum BLACK_KNIGHT_STATE {STATE_IDLE, STATE_FORWARD, STATE_BACKWARD, STATE_LEFT, STATE_RIGHT, STATE_TOBATTLE, STATE_TOIDLE, STATE_END};

private:
	CLothric_Knight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLothric_Knight(const CLothric_Knight& rhs);
	virtual ~CLothric_Knight() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CModel* m_pModelCom[STATE_END] = {nullptr};

public:
	const _uint Get_Souls() { return m_tParams.iSouls; }

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

private:

	NPC_PARAM m_tParams = {};
	BLACK_KNIGHT_STATE m_eCurrentState = { STATE_IDLE };
	BLACK_KNIGHT_STATE m_ePreviousState = { STATE_IDLE };
	vector<CChannel*>* m_pCurrentChannel;

	_tchar		m_szSouls[MAX_PATH] = TEXT("");

	_bool		m_bIsChanging = { false };




public:
	/* 원형객체를 생성할 때 */
	static CLothric_Knight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END