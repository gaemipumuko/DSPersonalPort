#include "..\Public\VIBuffer_Rect_Instancing.h"

CVIBuffer_Rect_Instancing::CVIBuffer_Rect_Instancing(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instancing(pDevice, pContext)
{

}

CVIBuffer_Rect_Instancing::CVIBuffer_Rect_Instancing(const CVIBuffer_Rect_Instancing & rhs)
	: CVIBuffer_Instancing(rhs)
{

}

HRESULT CVIBuffer_Rect_Instancing::Initialize_Prototype(_uint iNumInstance)
{
	

	m_iNumVertexBuffers = 2;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertices = 4;

	m_iIndexStride = 2;
	m_iNumInstance = iNumInstance;
	m_iIndexCountPerInstance = 6;
	m_iNumIndices = m_iIndexCountPerInstance * iNumInstance;

	m_iInstanceStride = sizeof(VTXINSTANCE);

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	
	ZeroMemory(&m_InitialData, sizeof m_InitialData);

	VTXPOSTEX*		pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	/* 로컬스페이스 상에서 너비높이가 1인 사각형을 만드랒. */
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.f);

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);

	_ushort*		pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	_uint		iNumIndices = 0;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 1;
		pIndices[iNumIndices++] = 2;

		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 2;
		pIndices[iNumIndices++] = 3;
	}

	

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

#pragma region INSTANCE_BUFFER
	ZeroMemory(&m_InstanceBufferDesc, sizeof m_InstanceBufferDesc);
	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;

	ZeroMemory(&m_InstanceInitialData, sizeof m_InstanceInitialData);

	VTXINSTANCE*		pVertexInstance = new VTXINSTANCE[m_iNumInstance];
	ZeroMemory(pVertexInstance, sizeof(VTXINSTANCE) * m_iNumInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertexInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pVertexInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pVertexInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pVertexInstance[i].vPosition = _float4(rand() % 10, 5.f, rand() % 10, 1.f);
	}

	m_InstanceInitialData.pSysMem = pVertexInstance;

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instancing::Initialize(void * pArg)
{
	
	if (nullptr != pArg)
	{
		PARTICLE_DESC*			pDesc = (PARTICLE_DESC*)pArg;
		uniform_real_distribution<float>		RandomX =
			uniform_real_distribution<float>(pDesc->vMinPosition.x, pDesc->vMaxPosition.x);
		uniform_real_distribution<float>		RandomY =
			uniform_real_distribution<float>(pDesc->vMinPosition.y, pDesc->vMaxPosition.y);
		uniform_real_distribution<float>		RandomZ =
			uniform_real_distribution<float>(pDesc->vMinPosition.z, pDesc->vMaxPosition.z);	

		uniform_real_distribution<float>		ScaleRandom = uniform_real_distribution<float>(pDesc->vScaleRange.x, pDesc->vScaleRange.y);
		uniform_real_distribution<float>		SpeedRandom = uniform_real_distribution<float>(pDesc->vSpeedRange.x, pDesc->vSpeedRange.y);
		uniform_real_distribution<float>		LifeTimeRandom = uniform_real_distribution<float>(pDesc->vLifeTime.x, pDesc->vLifeTime.y);
		

		VTXINSTANCE*		pVertices = (VTXINSTANCE*)m_InstanceInitialData.pSysMem;


		for (size_t i = 0; i < m_iNumInstance; i++)
		{
			_float		fScale = ScaleRandom(m_RandomNumber);

			pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
			pVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
			pVertices[i].vPosition = _float4(RandomX(m_RandomNumber), RandomY(m_RandomNumber), RandomZ(m_RandomNumber), 1.f);

			m_pSpeeds[i] = SpeedRandom(m_RandomNumber);
			m_pLifeTimes[i] = _float2(0.0f, LifeTimeRandom(m_RandomNumber));
		}
		int a = 10;
	}

#pragma region INSTANCE_BUFFER

	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceInitialData, &m_pVBInstance)))
		return E_FAIL;

#pragma endregion

	Safe_Delete_Array(m_InstanceInitialData.pSysMem);


	return S_OK;
}

CVIBuffer_Rect_Instancing * CVIBuffer_Rect_Instancing::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance)
{
	CVIBuffer_Rect_Instancing* pInstance = new CVIBuffer_Rect_Instancing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_Rect_Instancing::Clone(void * pArg)
{
	CVIBuffer_Rect_Instancing* pInstance = new CVIBuffer_Rect_Instancing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect_Instancing::Free()
{
	__super::Free();
}
