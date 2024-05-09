#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

/* 객체를 생성할 때 프로토타입을 이용하여 생성할꺼야. */
/* 프로토타입 : 실제 사용하기 위한 객체의 생성을 빠르게 하기 위해서.  */
/* - 로딩 간에 실제 사용하기위한 객체의 원형객체를 미리 생성한다. */
/* - 원형객체를 생성할 때 필요한 데이터 교류(서버와의 통신, 파일 입출력)를 마친다.  */
/* - 실제 사용할 객체를 생성할 때에는 원형객체의 데이터를 메모리 복사하여 생성한다. (오래걸리는 작업 => 메모리복사) */

/* 객체의 생성을 빠르게 하기 위해서. */
BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

/* ui형태로 그리는 직교투영을 통해서 그린다. */

class CTitle_Background final : public CGameObject
{
	enum Title_Texture { TITLE_UI_PRESS_ANY, TITLE_UI_STARTGAME, TITLE_UI_ENDGAME, TITLE_UI_SELECT, TITLE_UI_END};


private:
	CTitle_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTitle_Background(const CTitle_Background& rhs);
	virtual ~CTitle_Background() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*				m_pShaderCom				= { nullptr };	
	CTexture*				m_pBackgroundTextureCom		= { nullptr };
	CTexture*				m_pTextureCom[TITLE_UI_END] = { nullptr };
	CRenderer*				m_pRendererCom				= { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom				= { nullptr };
	
private:
	_float					m_fIndex = { 0.f };
	_float					m_fSizeX, m_fSizeY;
	_float					m_fX, m_fY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	_int					m_iCurrent_Slot = { 0 };
	_int					m_iCurrent_Page = { 1 };

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* 원형객체를 생성할 때 */
	static CTitle_Background* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 실사용객체를 생성할 때 : 원형객체가 호출하여 사본(실사용객체)을 리턴한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END