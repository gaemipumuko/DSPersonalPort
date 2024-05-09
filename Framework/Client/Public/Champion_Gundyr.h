#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "UI_Boss_Hp_Bar.h"
#include "UI_Boss_Bar_Base.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CChannel;
class CCollider;
END

BEGIN(Client)

class CGundyr final : public CGameObject
{
	enum YHORM_STATE {STATE_IDLE, STATE_ALERT, STATE_ATTACK, STATE_END};

private:
	CGundyr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGundyr(const CGundyr& rhs);
	virtual ~CGundyr() = default;

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
	CCollider* m_pColliderCom = { nullptr };
	CCollider* m_pWeaponColliderCom = { nullptr };

	CTransform* m_pPlayerTransform = { nullptr };

public:
	const _uint Get_Souls() { return m_tParams.iSouls; }

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();
	void	Determine_Behavior();
	void	Execute_Behavior(_float fTimeDelta);
	void	Execute_Animation(_uint iCurrentAnimIndex , _float fTimeDelta);

private:

	NPC_PARAM m_tParams = {};
	YHORM_STATE m_eCurrentState = { STATE_IDLE };
	YHORM_STATE m_ePreviousState = { STATE_IDLE };

	_tchar		m_szSouls[MAX_PATH] = TEXT("");

	_bool		m_bIsLoop;
	_bool		m_bIsChanging = { false };
	_bool		m_bIsFinished = { true };
	_bool		m_bIsAlert = { false };
	_bool		m_bMoveFinished = { true };
	_bool		m_bIsRotatable = { true };
	_bool		m_bRun = { false };
	_bool		m_bPhaseII = { false };
	_bool		m_bInvoke_Behavior = { true };

	_float		m_fBehaviorTimer = {};
	_float		m_fCurrentAnimPos = {};
	_float		m_fAtk_Interval = {0.f};
	_float		m_fIgnoreTimer = { 0.f };

	_int		m_iRandom = { 0 };

	_uint		m_iPrevAnimIndex = {62};
	_uint		m_iCurrentAnimIndex = {62};
	_uint		m_iNextAnimIndex = {62};
	_uint		m_iBehaviorStage = { };
	_uint		m_iBehaviorIndex = { };
	_uint		m_iAtkIndex = {0};

	_int		m_iIgnoreID = { -1 };

	CCollider::ATTACK_DESC m_tMonster_Attack = {};

	_vector		m_vInitialPos;

	/* RNG*/
private:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

	/*For.Monster UI*/
private:
	CBoss_Bar_Base* m_pBoss_Hp_Base = { nullptr };
	CBoss_Hp_Bar* m_pBoss_Hp_Bar = {nullptr};

public:
	/* 원형객체를 생성할 때 */
	static CGundyr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END