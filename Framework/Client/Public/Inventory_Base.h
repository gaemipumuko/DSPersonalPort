#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UI_Select_Square.h"

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

class CInventory_Base final : public CGameObject
{

private:
	CInventory_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory_Base(const CInventory_Base& rhs);
	virtual ~CInventory_Base() = default;

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

	CSelect_Square*			m_pSelect_Square = {};
private:
	_float					m_fIndex = { 0.f };
	_float					m_fSizeX, m_fSizeY;
	_float					m_fX, m_fY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	_tchar					m_szSouls[MAX_PATH] = TEXT("");

	_uint					m_iMenuIndex = {};

	PLAYER_PARAM*  m_pPlayerParam;

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* ������ü�� ������ �� */
	static CInventory_Base* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �ǻ�밴ü�� ������ �� : ������ü�� ȣ���Ͽ� �纻(�ǻ�밴ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END