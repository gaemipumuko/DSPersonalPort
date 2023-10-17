#pragma once

/* high level shader language : hlsl */
/* �ۼ��� ���̴� ������ �����Ͽ� ��ü(ID3DX11Effect)ȭ�Ѵ�. */
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
	/* Ư�� ���̴������� �������Ͽ� ��üȭ.  */
	/* ID3DX11Effect : ���̴� ���������� ������ ������. */
	/* �� ���̴��� �׸���. */
	ID3DX11Effect*					m_pEffect = { nullptr };
	_uint							m_iNumPasses = { 0 };

	/* ���̴��ȿ� ���ǵǾ��ִ� �н����� InputLayout�� ������. */
	/* ���̴� �ȿ��� ���� �׸������ϴ� ������ ��Ȯ�� �� �޾��� �� �ִ°�?! */
	vector<ID3D11InputLayout*>		m_InputLayouts;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END