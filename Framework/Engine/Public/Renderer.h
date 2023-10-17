#pragma once

#include "Component.h"

/* 1. 화면에 그려져야할 객체들을 가지고 있는다. */
/* 2. 객체들으 ㄹ보관할 때 그려지는 순서대로 구분하여 저장한다.  */
/* 3. 저장한 객체들의 렌더함수를 호출해준다. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP {RG_PRIORITY, RG_NONLIGHT, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };

	/* RG_PRIORITY : 하늘같이 먼저 추가해야하는 객체들을 추가해놓자. */
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