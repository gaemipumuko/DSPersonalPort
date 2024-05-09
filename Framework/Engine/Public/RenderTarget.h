#pragma once

#include "Base.h"

/* ����Ÿ���̴�. */

BEGIN(Engine)

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	ID3D11RenderTargetView* Get_RTV()  {
		return m_pRTV;
	}

public:
	HRESULT Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vColor);
	void Clear();
	HRESULT Bind_RTV(class CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

#endif


private:
	ID3D11Device*							m_pDevice = { nullptr };
	ID3D11DeviceContext*					m_pContext = { nullptr };

	/* �ؽ��ķκ��� ����Ÿ��, ���̴����ҽ��� ������. */
	ID3D11Texture2D*						m_pTexture2D = { nullptr };

	/* ����Ÿ���� �뵵 : �� �ؽ��Ŀ� ������ �׸���.  */
	ID3D11RenderTargetView*					m_pRTV = { nullptr };

	/* ���̴����ҽ��� �뵵 : �� �ؽ��ĸ� ���̴��� ������ ���ø� �ؿ� �� �ֵ��� �ϰڴ�. */
	ID3D11ShaderResourceView*				m_pSRV = { nullptr };

	_float4									m_vClearColor = {};

#ifdef _DEBUG
private:
	_float4x4				m_WorldMatrix;
#endif

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vColor);
	virtual void Free() override;
};

END