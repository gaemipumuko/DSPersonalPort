#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_iVertexStride(rhs.m_iVertexStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iIndexStride(rhs.m_iIndexStride)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_ePrimitiveTopology(rhs.m_ePrimitiveTopology)
{
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	ID3D11Buffer*		pVertexBuffer[] = {
		m_pVB, 		
	};

	_uint				iVertexStrides[] = {
		m_iVertexStride, 
	};

	_uint				iOffsets[] = {
		0, 
	};

	/* 어떤 정점버퍼들을 장치에 바인딩할꺼야?! 동시에 여러개의 정점버퍼를 바인딩할 수 있다. */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffer, iVertexStrides, iOffsets);

	/* 인덱스버퍼를 바인딩하자. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 어떤 방식으로 그려나갈꺼야?! */
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	//ID3D11InputLayout*		pInputLayout;

	/* D3D11_INPUT_ELEMENT_DESC : 정점의 구성정보를 배열로 표혆나다. */
	/* NumElement : 그래서 위에서 선언한 배열이 몇개야?! */

	


	/* 내정점이 이렇게 생겼다! */
	/* 
	LPCSTR SemanticName;
	UINT SemanticIndex;
	DXGI_FORMAT Format;
	UINT InputSlot;
	UINT AlignedByteOffset;
	D3D11_INPUT_CLASSIFICATION InputSlotClass;
	UINT InstanceDataStepRate;
	*/
	//D3D11_INPUT_ELEMENT_DESC	VertexElements[2] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, /* Position */
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}, /* Texcoord */		
	//	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}, /* Texcoord */		
	//};

	//// m_pDevice->CreateInputLayout(VertexElements, 2, ShaderByteCode, ShaderByteCodelength, &pInputLayout);
	//m_pContext->IASetInputLayout(pInputLayout);

	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer ** ppBuffer)
{
	/* D3D11_BUFFER_DESC : 지금 할당할려고하는 버퍼(메모리공간)의 속성을 정의한다. */
	/* D3D11_SUBRESOURCE_DATA : 지금 할당한 메모리 공간에 어떤 값을 채울까?! */

	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, ppBuffer);	
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

}
