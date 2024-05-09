#include "..\Public\GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Font_Manager.h"
#include "Frustum.h"
#include "Target_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}


HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, HINSTANCE hInstance, const GRAPHIC_DESC & GraphicDesc, ID3D11Device ** ppDevice, ID3D11DeviceContext ** ppContext)
{
	/* 타이머를 생성하나다. */
	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* 그래픽 디바이스를 초기화한다. */
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc.hWnd, GraphicDesc.isWindowed, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(hInstance, GraphicDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	/* 레벨 매니져를 초기화한다. */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	/* 오브젝트 매니져를 초기화 한다. */
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* 컴포넌트 매니져를 초기화 한다. */
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create(iNumLevels);
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager || 
		nullptr == m_pPipeLine || 
		nullptr == m_pInput_Device)
		return;

	/* 키보드와 마우스의 상태를 저장해놓는다.*/
	m_pInput_Device->Update_InputDev();

	m_pObject_Manager->Tick(fTimeDelta);

	m_pPipeLine->Tick();

	m_pFrustum->Tick();

	m_pObject_Manager->LateTick(fTimeDelta);

	/* 할거없ㄷ어. */
	/* 각 레벨에서 띄우고 싶은 디버깅적인 요소?! */
	m_pLevel_Manager->Tick(fTimeDelta);
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	m_pLight_Manager->Clear(iLevelIndex);
	m_pObject_Manager->Clear(iLevelIndex);
	m_pComponent_Manager->Clear(iLevelIndex);

}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);

	return S_OK;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}


_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID) const
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse) const
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState) const
{
	if (nullptr == m_pInput_Device)
		return 0l;

	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

_bool CGameInstance::Get_Key_Down(_int _iKey)
{
	return m_pInput_Device->Get_Key_Down(_iKey);
}

_bool CGameInstance::Get_Key_Press(_int _iKey)
{
	return m_pInput_Device->Get_Key_Press(_iKey);
}

_bool CGameInstance::Get_Key_Up(_int _iKey)
{
	return m_pInput_Device->Get_Key_Up(_iKey);
}

_bool CGameInstance::Key_Down(int byKeyID)
{
	return m_pInput_Device->Key_Down(byKeyID);
}

_bool CGameInstance::Key_Press(int byKeyID)
{
	return m_pInput_Device->Key_Press(byKeyID);
}

_bool CGameInstance::Key_Up(int byKeyID)
{
	return m_pInput_Device->Key_Release(byKeyID);
}


HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);	
}

_float CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);	
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();
}

_int CGameInstance::Get_CurrentLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return -1;

	return m_pLevel_Manager->Get_CurrentLevelIndex();
}

_int CGameInstance::Get_NextLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return -1;

	return m_pLevel_Manager->Get_NextLevelIndex();
}

void CGameInstance::Set_NextLevelIndex(_uint iLevelIndex)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pLevel_Manager->Set_NextLevelIndex(iLevelIndex);
}


_bool CGameInstance::Change_Level()
{
	return m_pLevel_Manager->Get_Change_Level();
}

_bool CGameInstance::Get_isLoaded()
{
	return m_pLevel_Manager->Get_isLoaded();
}

void CGameInstance::Ready_Next_Level(_uint iLevelIndex, _bool bLoaded)
{
	m_pLevel_Manager->Ready_Next_Level(iLevelIndex, bLoaded);
}

_int CGameInstance::Get_Reserved_Level_Index()
{
	return m_pLevel_Manager->Get_ReservedLevelIndex();
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_Layers(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Layers(iLevelIndex, strLayerTag, strPrototypeTag, pArg);	
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex, _int iPartObjectIndex)
{
		if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex, iPartObjectIndex);
}

CGameObject* CGameInstance::Get_Object(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strObjectName, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Object(iLevelIndex, strLayerTag, strObjectName, iIndex);
}

CGameObject* CGameInstance::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_GameObject(strPrototypeTag, pArg);
}

_uint CGameInstance::Get_NumObjects(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return 0;

	return m_pObject_Manager->Get_NumObjects(iLevelIndex, strLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

_float4 CGameInstance::Get_CamPosition() const
{
	if (nullptr == m_pPipeLine)
		return _float4(0.f, 0.f, 0.f, 0.f);

	return m_pPipeLine->Get_CamPosition();	
}
_float4 CGameInstance::Get_CamLook() const
{
	if (nullptr == m_pPipeLine)
		return _float4(0.f, 0.f, 0.f, 0.f);

	return m_pPipeLine->Get_CamLook();
}

_float CGameInstance::Get_CamFar() const 
{
	if (nullptr == m_pPipeLine)
		return 0.f;

	return m_pPipeLine->Get_CamFar();
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_Transform_Float4x4(eTransformState);
}

_float4x4 CGameInstance::Get_Transform_Inverse_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_Transform_Inverse_Float4x4(eTransformState);
}

_matrix CGameInstance::Get_Transform(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform(eTransformState);
}

_matrix CGameInstance::Get_Transform_Inverse(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform_Inverse(eTransformState);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, const _float4x4 & TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eTransformState, TransformMatrix);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;
	m_pPipeLine->Set_Transform(eTransformState, TransformMatrix);
}

void CGameInstance::Set_CameraFar(_float fFar)
{
	m_pPipeLine->Set_CameraFar(fFar);
}

HRESULT CGameInstance::Add_Light(_uint iLevelIndex, const LIGHT_DESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(iLevelIndex, LightDesc);
}

const LIGHT_DESC * CGameInstance::Get_LightDesc(_uint iLevelIndex, _uint iIndex) const
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iLevelIndex, iIndex);
}

HRESULT CGameInstance::Render_Lights(_uint iLevelIndex, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Render(iLevelIndex, pShader, pVIBuffer);
}

HRESULT CGameInstance::Add_Font(const wstring& strFontTag, const wstring& strFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(strFontTag, strFontFilePath);
}

HRESULT CGameInstance::Render_Text(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, _float fScale, _fvector vColor, _float fRotation, const _float2& vOrigin)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render(strFontTag, strText, vPosition, fScale, vColor, fRotation, vOrigin);
}

_bool CGameInstance::isIn_Frustum_InWorldSpace(_fvector vWorldPos, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_Frustum_InWorldSpace(vWorldPos, fRange);
}

HRESULT CGameInstance::Add_RenderTarget(const wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vColor)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vColor);
}

HRESULT CGameInstance::Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSV)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->Begin_MRT(strMRTTag);
}

HRESULT CGameInstance::End_MRT()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RTV(CShader* pShader, _char* pConstantName, const wstring& strTargetTag)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->Bind_RTV(pShader, pConstantName, strTargetTag);
}

#ifdef _DEBUG

HRESULT CGameInstance::Ready_RTDebug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RTDebug(const wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}

#endif

void CGameInstance::Clear_Manager()
{
	Safe_Release(m_pTarget_Manager);

	Safe_Release(m_pFrustum);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pComponent_Manager);	

	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pLight_Manager);

	Safe_Release(m_pFont_Manager);

}

void CGameInstance::Release_Engine()
{	
	/* 실제 매니져들의 파괴. */
	CGameInstance::Get_Instance()->Clear_Manager();

	/* 게임인스턴스를 파괴한다. */
	CGameInstance::Destroy_Instance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
