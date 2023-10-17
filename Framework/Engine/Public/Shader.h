#pragma once

/* high level shader language : hlsl */
/* 작성한 셰이더 파일을 빌드하여 객체(ID3DX11Effect)화한다. */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg);	



public:
	HRESULT Begin(_uint iPassIndex);

	HRESULT Bind_ShaderResourceView(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_ShaderResourceViews(const _char* pConstantName, ID3D11ShaderResourceView** pSRVs, _uint iNumTextures);

	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4& Matrix);
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices);

	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iByteLength);

private:
	/* 특정 셰이더파일을 컴파일하여 객체화.  */
	/* ID3DX11Effect : 셰이더 전역변수에 값ㅇ르 던진다. */
	/* 이 셰이더로 그린다. */
	ID3DX11Effect*					m_pEffect = { nullptr };
	_uint							m_iNumPasses = { 0 };

	/* 셰이더안에 정의되어있는 패스마다 InputLayout을 만들자. */
	/* 셰이더 안에서 내가 그릴려고하는 정점을 정확히 잘 받아줄 수 있는가?! */
	vector<ID3D11InputLayout*>		m_InputLayouts;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END