#pragma once

#include "Base.h"

/* 렌더타겟이다. */

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

	/* 텍스쳐로부터 렌더타겟, 셰이더리소스를 만들어낸다. */
	ID3D11Texture2D*						m_pTexture2D = { nullptr };

	/* 렌더타겟의 용도 : 이 텍스쳐에 뭔가를 그린다.  */
	ID3D11RenderTargetView*					m_pRTV = { nullptr };

	/* 셰이더리소스의 용도 : 이 텍스쳐를 셰이더로 던져서 샘플링 해올 수 있도록 하겠다. */
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