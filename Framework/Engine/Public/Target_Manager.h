#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{	
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderTarget(const wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vColor);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);

	/* 모아놓은 렌더타겟들(그룹)을 장치에 동시에 한꺼번에 바인딩한다 .*/
	HRESULT Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDepthStencilView = nullptr);

	/* 원상복구한다.(백버퍼를 다시 0번째 위치에 바인딩한다. ) */
	HRESULT End_MRT();
	HRESULT Bind_RTV(class CShader* pShader, _char* pConstantName, const wstring& strTargetTag);

#ifdef _DEBUG

public:
	HRESULT Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const wstring& strMTRTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

#endif


private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

private:
	ID3D11RenderTargetView*			m_pBackBufferRTV = { nullptr };
	ID3D11DepthStencilView*			m_pMainDepthStencilView = { nullptr };

private: /* 생성한 렌더타겟들은 우선 여기에 보관한다. */
	map<const wstring, class CRenderTarget*>			m_RenderTargets;

private: /* 장치에 동시에 바인딩되어야할 타겟드을 미리 모아두자. */
	map<const wstring, list<class CRenderTarget*>>		m_MRTs; 

private:
	class CRenderTarget* Find_RenderTarget(const wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring& strMRTTag);




public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};


END