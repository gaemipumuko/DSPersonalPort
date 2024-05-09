#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

/* ��ü�� ������ �� ������Ÿ���� �̿��Ͽ� �����Ҳ���. */
/* ������Ÿ�� : ���� ����ϱ� ���� ��ü�� ������ ������ �ϱ� ���ؼ�.  */
/* - �ε� ���� ���� ����ϱ����� ��ü�� ������ü�� �̸� �����Ѵ�. */
/* - ������ü�� ������ �� �ʿ��� ������ ����(�������� ���, ���� �����)�� ��ģ��.  */
/* - ���� ����� ��ü�� ������ ������ ������ü�� �����͸� �޸� �����Ͽ� �����Ѵ�. (�����ɸ��� �۾� => �޸𸮺���) */

/* ��ü�� ������ ������ �ϱ� ���ؼ�. */
BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

/* ui���·� �׸��� ���������� ���ؼ� �׸���. */

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
	/* ������ü�� ������ �� */
	static CTitle_Background* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �ǻ�밴ü�� ������ �� : ������ü�� ȣ���Ͽ� �纻(�ǻ�밴ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END