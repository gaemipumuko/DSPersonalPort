#include "VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing & rhs)
	: CVIBuffer(rhs)
	, m_InstanceBufferDesc(rhs.m_InstanceBufferDesc)
	, m_InstanceInitialData(rhs.m_InstanceInitialData)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_iIndexCountPerInstance(rhs.m_iIndexCountPerInstance)
	, m_iNumInstance(rhs.m_iNumInstance)	
	, m_RandomNumber(rhs.m_RandomNumber)
	, m_pSpeeds(rhs.m_pSpeeds)
	, m_pLifeTimes(rhs.m_pLifeTimes)
{

	VTXINSTANCE* pVertexInstance = new VTXINSTANCE[rhs.m_iNumInstance];
	ZeroMemory(pVertexInstance, sizeof(VTXINSTANCE) * rhs.m_iNumInstance);

	memcpy(pVertexInstance, rhs.m_InstanceInitialData.pSysMem, sizeof(VTXINSTANCE) * rhs.m_iNumInstance);
	m_InstanceInitialData.pSysMem = pVertexInstance;
}

HRESULT CVIBuffer_Instancing::Initialize_Prototype()
{
	m_RandomNumber = mt19937_64(m_RandomDevice());

	m_pSpeeds = new _float[m_iNumInstance];
	m_pLifeTimes = new _float2[m_iNumInstance];

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Instancing::Update(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_pSpeeds[i] * fTimeDelta;
		m_pLifeTimes[i].x += fTimeDelta;

		if (m_pLifeTimes[i].y < m_pLifeTimes[i].x) 
		{
			m_pLifeTimes[i].x = 0.f;
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.y = 0.f;
		}


	}
	

	m_pContext->Unmap(m_pVBInstance, 0);

	
}

HRESULT CVIBuffer_Instancing::Render()
{
	ID3D11Buffer*		pVertexBuffer[] = {
		m_pVB,
		m_pVBInstance, 
	};

	_uint				iVertexStrides[] = {
		m_iVertexStride,
		m_iInstanceStride, 

	};

	_uint				iOffsets[] = {
		0,
		0, 
	};

	/* 어떤 정점버퍼들을 장치에 바인딩할꺼야?! 동시에 여러개의 정점버퍼를 바인딩할 수 있다. */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffer, iVertexStrides, iOffsets);

	/* 인덱스버퍼를 바인딩하자. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 어떤 방식으로 그려나갈꺼야?! */
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}


void CVIBuffer_Instancing::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pLifeTimes);		
		Safe_Delete_Array(m_pSpeeds);
	}

	Safe_Release(m_pVBInstance);

}
