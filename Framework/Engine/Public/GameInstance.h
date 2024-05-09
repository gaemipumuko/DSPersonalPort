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
	_bool	Get_Key_Down(_int _iKey);
	_bool	Get_Key_Press(_int _iKey);
	_bool	Get_Key_Up(_int _iKey);
	_bool	Key_Down(int byKeyID);
	_bool	Key_Press(int byKeyID);
	_bool	Key_Up(int byKeyID);

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const wstring& strTimerTag);
	_float Compute_TimeDelta(const wstring& strTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);	
	HRESULT Render_Level();
	_int	Get_CurrentLevelIndex();
	_int	Get_NextLevelIndex();
	void	Set_NextLevelIndex(_uint iLevelIndex);
	void	Ready_Next_Level(_uint iLevelIndex, _bool bLoaded = false);
	_int	Get_Reserved_Level_Index();
	_bool	Change_Level();
	_bool   Get_isLoaded();

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject*);
	HRESULT Add_Layers(_uint iLevelIndex, const wstring& LayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex = 0 ,_int iPartObjectIndex = -1);
	CGameObject* Get_Object(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strObjectName, _uint iIndex = 0);
	class CGameObject* Clone_GameObject(const wstring& strPrototypeTag, void* pArg = nullptr);
	_uint Get_NumObjects(_uint iLevelIndex, const wstring& strLayerTag);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.PipeLine*/
	_float4 Get_CamPosition() const;
	_float4 Get_CamLook() const;
	_float Get_CamFar() const;
	_float4x4 Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_float4x4 Get_Transform_Inverse_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_matrix Get_Transform_Inverse(CPipeLine::TRANSFORMSTATE eTransformState) const;
	void Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, const _float4x4& TransformMatrix);
	void Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, _fmatrix TransformMatrix);
	void Set_CameraFar(_float fFar);

public: /* For.Light_Manager */
	HRESULT Add_Light(_uint iLevelIndex, const LIGHT_DESC& LightDesc);
	const LIGHT_DESC* Get_LightDesc(_uint iLevelIndex, _uint iIndex) const;
	HRESULT Render_Lights(_uint iLevelIndex, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

public: /* For.Font_Manager */
	HRESULT Add_Font(const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT Render_Text(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, _float fScale = 1.f, _fvector vColor = Colors::White, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f));

public: /* For.Frustum*/
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPos, _float fRange = 0.f);

public: /* For.Target_Manager */
	HRESULT Add_RenderTarget(const wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vColor);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr);
	HRESULT End_MRT();
	HRESULT Bind_RTV(class CShader* pShader, _char* pConstantName, const wstring& strTargetTag);
#ifdef _DEBUG
	HRESULT Ready_RTDebug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RTDebug(const wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif


private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };
	class CFont_Manager*			m_pFont_Manager = {};
	class CFrustum*					m_pFrustum = { nullptr };
	class CTarget_Manager*			m_pTarget_Manager = { nullptr };

private:
	void Clear_Manager();

public:
	static void Release_Engine();

public:
	virtual void Free() override;
};

END