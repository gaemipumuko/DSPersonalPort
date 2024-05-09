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

class CMidir final : public CGameObject
{
	enum MIDYR_STATE {STATE_IDLE, STATE_BUFF, STATE_BUFF_WHILE, STATE_BUFF_END, STATE_SALUTE, STATE_SHY, STATE_TURN, STATE_END};

private:
	CMidir(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMidir(const CMidir& rhs);
	virtual ~CMidir() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CModel* m_pModelCom = {nullptr};

public:
	const _uint Get_Souls() { return m_tParams.iSouls; }
private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

private:
	_bool		m_bIsApproched = { true };


	NPC_PARAM m_tParams = {};
	MIDYR_STATE m_eCurrentState = { STATE_IDLE };
	MIDYR_STATE m_ePreviousState = { STATE_IDLE };

	_tchar		m_szSouls[MAX_PATH] = TEXT("");





public:
	/* 원형객체를 생성할 때 */
	static CMidir* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END