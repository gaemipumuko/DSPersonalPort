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
//C3190
class CBlackSmith_Andre final : public CGameObject
{
	enum ANDRE_STATE {STATE_IDLE, STATE_ATTACK, STATE_BUFF_WHILE, STATE_BUFF_END, STATE_SALUTE, STATE_SHY, STATE_TURN, STATE_END};

private:
	CBlackSmith_Andre(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlackSmith_Andre(const CBlackSmith_Andre& rhs);
	virtual ~CBlackSmith_Andre() = default;

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
	_bool		m_bIsApproched = { true };


	NPC_PARAM m_tParams = {};
	ANDRE_STATE m_eCurrentState = { STATE_IDLE };
	ANDRE_STATE m_ePreviousState = { STATE_IDLE };
	vector<CChannel*>* m_pCurrentChannel;

	_tchar		m_szSouls[MAX_PATH] = TEXT("");





public:
	/* ������ü�� ������ �� */
	static CBlackSmith_Andre* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �ǻ�밴ü�� ������ �� : ������ü�� ȣ���Ͽ� �纻(�ǻ�밴ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END