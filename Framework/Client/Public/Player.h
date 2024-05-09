#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CChannel;
class CCollider;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public: 
	enum PLAYER_DIR {DIR_FRONT,DIR_FRONT_RIGHT,DIR_RIGHT,DIR_BACK_RIGHT,DIR_BACK,DIR_BACK_LEFT,DIR_LEFT,DIR_FRONT_LEFT,DIR_END};

	enum PLAYER_MOVESET {MOVE_DAGGER,MOVE_SWORD,MOVE_GREATSWORD,MOVE_LANCE,MOVE_END};

	enum PART { PART_ARM,PART_BODY, PART_HEAD, PART_LEG, PART_RHAND, PART_END };

	enum PLAYER_STATE {STATE_IDLE,STATE_MOVE,STATE_INVINSIVLE,STATE_ATTACK,STATE_END};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual class CComponent* Find_Component(const wstring& strComTag, _int iPartObjectIndex = -1) override;

private:
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CModel* m_pModelCom = {nullptr};
	CCollider* m_pColliderCom = { nullptr };

private:
	class CGameObject* m_Parts[PART_END] = {};

public:
	_bool Restore_Hp(_float fHp);
	const _uint Get_Souls() { return m_tPlayerParams.iSouls; }
	PLAYER_PARAM* Get_PlayerParam() { return &m_tPlayerParams; }
	const PLAYER_STATE& Get_PlayerState() { return m_eCurrentState; }
private:
	HRESULT Add_Component();
	HRESULT Add_Parts();
	HRESULT Bind_ShaderResources();
	void	SetDirection(PLAYER_DIR eDirection, _float fTimeDelta);
	void	Execute_Animation(_uint iCurrentAnimIndex, _float fTimeDelta);
	void	SetUpOnTerrain();

private:

	PLAYER_PARAM m_tPlayerParams = {};
	PLAYER_DIR			m_eCurrentDir = DIR_FRONT;
	PLAYER_MOVESET		m_eCurrentMoveset = MOVE_SWORD;
	PLAYER_STATE		m_eCurrentState = STATE_END;
	vector<CChannel*>* m_pCurrentChannel;

	_tchar		m_szSouls[MAX_PATH] = TEXT("");

	_bool		m_bIsTwoHanded = { false };
	_bool		m_bIsChanging = { false };
	_bool		m_bIsLoop = { false };
	_bool		m_bIsFinished = {true};
	_bool		m_bInputCheck = { true };
	_bool		m_bCancelEnable = {false};
	_bool		m_bResetTimer = { false };
	_bool		m_bisRunning = { false };

	_float		m_fSInputDelay = { 0.3f };
	_float		m_fInputDelay = {0.f};
	_float		m_fCurrentAngle = {};
	_float		m_fCurrentAnimPos = { 0.f };

	_int		m_iNextAnimIndex = {-1};

	_uint		m_iCurrentAnimIndex = {75};
	_uint		m_iPrevAnimIndex = {75};

	CCollider::ATTACK_DESC m_tPlayer_Attack = {};

public:
	/* 원형객체를 생성할 때 */
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END