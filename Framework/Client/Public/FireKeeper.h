#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Interaction.h"
#include "UI_NPC_Base.h"
#include "UI_Dialogue_BG.h"
#include "UI_Script_FireKeeper.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CChannel;
END

BEGIN(Client)

class CFire_Keeper final : public CGameObject
{
	enum FIRE_KEEPER_STATE {STATE_IDLE, STATE_BUFF, STATE_BUFF_WHILE, STATE_BUFF_END, STATE_SALUTE, STATE_SHY, STATE_TURN, STATE_END};
	enum FIRE_KEEPER_DIALOGUE_STATE { STATE_DORMANT, STATE_ADDRESSED, STATE_SELECTING, STATE_LV_UP, STATE_TALKING, DIALOGUE_STATE_END};


private:
	CFire_Keeper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFire_Keeper(const CFire_Keeper& rhs);
	virtual ~CFire_Keeper() = default;

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

	CUI_NPC_Base* m_pNPCBase = {nullptr};
	CInteraction* m_pInteraction_Box = {nullptr};
	CUI_Dialogue_BG* m_pDialogueBG = { nullptr };
	CUI_Script_Firekeeper* m_pScript = { nullptr };
public:
	const _uint Get_Souls() { return m_tParams.iSouls; }
private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

private:
	_bool		m_bIsApproched = { true };
	_bool		m_bIsTalking = {false};
	_bool		m_bIsSpeaking = { false };
	
	_uint		m_iDialogueIdx = {};
	_uint		m_iSelectionIdx = {};

	NPC_PARAM m_tParams = {};
	FIRE_KEEPER_STATE m_eCurrentState = { STATE_IDLE };
	FIRE_KEEPER_STATE m_ePreviousState = { STATE_IDLE };
	FIRE_KEEPER_DIALOGUE_STATE m_eDialogueState = { DIALOGUE_STATE_END };
	vector<CChannel*>* m_pCurrentChannel;

	_tchar		m_szSouls[MAX_PATH] = TEXT("");





public:
	/* 원형객체를 생성할 때 */
	static CFire_Keeper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END