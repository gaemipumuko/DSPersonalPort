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

class CInteraction final : public CGameObject
{

private:
	CInteraction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInteraction(const CInteraction& rhs);
	virtual ~CInteraction() = default;

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
	_tchar					m_szText[MAX_PATH] = TEXT("");

	_float					m_fIndex = { 0.f };
	_float					m_fSizeX, m_fSizeY;
	_float					m_fX, m_fY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

public:
	/* ������ü�� ������ �� */
	static CInteraction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);

	/* �ǻ�밴ü�� ������ �� : ������ü�� ȣ���Ͽ� �纻(�ǻ�밴ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END