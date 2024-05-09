#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent 
{
public:
	enum ATK_TYPE { ATK_LIGHT,ATK_NORMAL, ATK_HEAVY, ATK_PARRY, ATK_DOWN, ATK_AIRBOURN, ATK_END };
	enum COL_STATE { STATE_NORMAL, STATE_INACTIVE, STATE_PARRY, STATE_END};

	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct Attack_Behavior_Desc
	{
		_uint iBehaviorID = { 0 };
		_float fDamage = { 0.f };
		_float	fPostureDamage = { 0.f };
		ATK_TYPE eAttackType = ATK_END;

	}ATTACK_DESC;

/*Informations To Receieve From opponent's Collider

Position of the Collider -> To Calculate Col Direction
Attack Type -> Light, Normal, Heavy & Thrust , Slash, Strike , Standard  & Parry
Rigidity Damage
Collider State -> Normal, Inactive, Parrying, & etc;
=======================================================
Things Consider in order to Deal Damage Only Once during Active Time.

When Invoking Attack Behavior, Provide ID to The Behavior.
For Brief Amount of Time, Mosnters Do Not Get Damage form Same ID.

Player, Only Checks Active State.

==========================
Force Parry
Player Has Parry Collider.
But During Force Parry, Instead of Parry Collider, Player Body Turns to Parry state.
===========================
Was Going to Make Colliders Have ATTACK_DESC Structure,
but Since it seemed like it is a waste of memory if every colliders Carried Them.
Instead Obejects that Need to invoke attack behavior will have the ATTACK_DESC
and pass on ptr to the collider
*/


private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType);
	virtual HRESULT Initialize(void* pArg) override;
	void Update(_fmatrix WorldMatrix);

public:
	_bool Intersect(const class CCollider* pTargetCollider);
	void Ready_Attack_Behavior(ATTACK_DESC* AttackDesc) { if (AttackDesc != nullptr) { m_pAttackDesc = AttackDesc; } }
	void Activate_Collider() { m_eColState = STATE_NORMAL; }
	void Inactivate_Collider() { m_eColState = STATE_INACTIVE; }
	const COL_STATE Get_Collider_State() { return m_eColState; }
	ATTACK_DESC Get_Attack_Desc() { return *m_pAttackDesc; }
	_vector Get_Collider_Position();
	_float Get_Collider_Radius();
	

#ifdef _DEBUG
public:
	HRESULT Render() override;
#endif

private:
	TYPE				m_eType = { TYPE_END };
	class CBounding* m_pBounding = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };

	COL_STATE		m_eColState = { STATE_END };

	ATTACK_DESC* m_pAttackDesc = { nullptr };

#ifdef _DEBUG
private:
	PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };

#endif 

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END