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

	/* � �������۵��� ��ġ�� ���ε��Ҳ���?! ���ÿ� �������� �������۸� ���ε��� �� �ִ�. */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffer, iVertexStrides, iOffsets);

	/* �ε������۸� ���ε�����. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* � ������� �׷���������?! */
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	//ID3D11InputLayout*		pInputLayout;

	/* D3D11_INPUT_ELEMENT_DESC : ������ ���������� �迭�� ǥ�q����. */
	/* NumElement : �׷��� ������ ������ �迭�� ���?! */

	


	/* �������� �̷��� �����! */
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
	/* D3D11_BUFFER_DESC : ���� �Ҵ��ҷ����ϴ� ����(�޸𸮰���)�� �Ӽ��� �����Ѵ�. */
	/* D3D11_SUBRESOURCE_DATA : ���� �Ҵ��� �޸� ������ � ���� ä���?! */

	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, ppBuffer);	
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

}
