#pragma once

#include "Component_Manager.h"
#include "PipeLine.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(_uint iNumLevels, HINSTANCE hInstance, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);
	void Clear(_uint iLevelIndex); /* 기존 레벨에서 사용했던 자우너들을 삭제한다. */
	
public: /* For.Graphic_Device*/ 
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();

public: /* For.Input_Device */
	_byte	Get_DIKeyState(_ubyte byKeyID) const;
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) const;
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	const;

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const wstring& strTimerTag);
	_float Compute_TimeDelta(const wstring& strTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);	
	HRESULT Render_Level();

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject*);
	HRESULT Add_Layers(_uint iLevelIndex, const wstring& LayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.PipeLine*/
	_float4 Get_CamPosition() const;
	_float4x4 Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_float4x4 Get_Transform_Inverse_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_matrix Get_Transform_Inverse(CPipeLine::TRANSFORMSTATE eTransformState) const;
	void Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, const _float4x4& TransformMatrix);
	void Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, _fmatrix TransformMatrix);

public: /* For.Light_Manager */
	HRESULT Add_Light(_uint iLevelIndex, const LIGHT_DESC& LightDesc);
	const LIGHT_DESC* Get_LightDesc(_uint iLevelIndex, _uint iIndex) const;





private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };

private:
	void Clear_Manager();

public:
	static void Release_Engine();

public:
	virtual void Free() override;
};

END