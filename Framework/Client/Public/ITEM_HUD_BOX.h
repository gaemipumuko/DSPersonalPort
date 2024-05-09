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

class CItem_HUD_Box final : public CGameObject
{
public:
	enum BOX_SPACE { BOX_LEFT, BOX_RIGHT, BOX_TOP, BOX_BOTTOM, BOX_END };

private:
	CItem_HUD_Box(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_HUD_Box(const CItem_HUD_Box& rhs);
	virtual ~CItem_HUD_Box() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*				m_pShaderCom = { nullptr };	
	CTexture*				m_pTextureCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	
private:
	_float					m_fIndex = { 0.f };
	_float					m_fSizeX, m_fSizeY;
	_float					m_fX, m_fY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	
	BOX_SPACE				m_eBoxSpace = {BOX_END};

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* ������ü�� ������ �� */
	static CItem_HUD_Box* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �ǻ�밴ü�� ������ �� : ������ü�� ȣ���Ͽ� �纻(�ǻ�밴ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END