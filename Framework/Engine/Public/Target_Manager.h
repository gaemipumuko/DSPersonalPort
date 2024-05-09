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

	/* ��Ƴ��� ����Ÿ�ٵ�(�׷�)�� ��ġ�� ���ÿ� �Ѳ����� ���ε��Ѵ� .*/
	HRESULT Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDepthStencilView = nullptr);

	/* ���󺹱��Ѵ�.(����۸� �ٽ� 0��° ��ġ�� ���ε��Ѵ�. ) */
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

private: /* ������ ����Ÿ�ٵ��� �켱 ���⿡ �����Ѵ�. */
	map<const wstring, class CRenderTarget*>			m_RenderTargets;

private: /* ��ġ�� ���ÿ� ���ε��Ǿ���� Ÿ�ٵ��� �̸� ��Ƶ���. */
	map<const wstring, list<class CRenderTarget*>>		m_MRTs; 

private:
	class CRenderTarget* Find_RenderTarget(const wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring& strMRTTag);




public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};


END