#pragma once

#include "Component.h"

/* 1. ȭ�鿡 �׷������� ��ü���� ������ �ִ´�. */
/* 2. ��ü���� �������� �� �׷����� ������� �����Ͽ� �����Ѵ�.  */
/* 3. ������ ��ü���� �����Լ��� ȣ�����ش�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP {RG_PRIORITY, RG_NONLIGHT, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };

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

private:
	list<class CGameObject*>			m_RenderObjects[RG_END];

private:	
	HRESULT Render_Priority();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();	

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END