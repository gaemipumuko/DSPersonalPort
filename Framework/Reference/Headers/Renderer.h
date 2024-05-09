#pragma once

#include "Component.h"

/* 1. ȭ�鿡 �׷������� ��ü���� ������ �ִ´�. */
/* 2. ��ü���� �������� �� �׷����� ������� �����Ͽ� �����Ѵ�.  */
/* 3. ������ ��ü���� �����Լ��� ȣ�����ش�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONLIGHT, RG_SHADOW, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };

	/* RG_PRIORITY : �ϴð��� ���� �߰��ؾ��ϴ� ��ü���� �߰��س���. */
	/* RG_NONBLEND : */
	/* RG_BLEND : */
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderer(const CRenderer& rhs) = delete;
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw_RenderGroup();

#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pDebugComponent);
#endif

private:
	list<class CGameObject*>			m_RenderObjects[RG_END];

#ifdef _DEBUG
private:
	list<class CComponent*>				m_DebugComponent;

#endif

private:
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	class CShader* m_pShader = { nullptr };

	_float4x4							m_WorldMatrix;
	_float4x4							m_ViewMatrix, m_ProjMatrix;

	ID3D11DepthStencilView* m_pShadowDSV = { nullptr };

private:	
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	HRESULT Render_NonBlend();
	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();
	HRESULT Render_NonLight();
	HRESULT Render_Blend();
	HRESULT Render_UI();	

#ifdef _DEBUG
	HRESULT Render_Debug();
#endif

private:
	HRESULT Ready_ShadowDepthStencilView(_uint iSizeX, _uint iSizeY);

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END