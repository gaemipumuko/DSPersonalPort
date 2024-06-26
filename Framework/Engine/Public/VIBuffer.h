#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

protected:
	/* 정점과 인덱스를 배열형태로 할당한다. */
	ID3D11Buffer*						m_pVB = { nullptr };
	ID3D11Buffer*						m_pIB = { nullptr };

	_float3*							m_pVerticesPos = { nullptr };                      
	_uint*								m_pIndices = { nullptr };

	_uint								m_iNumVertexBuffers = { 0 };
	
	_uint								m_iVertexStride = { 0 };
	_uint								m_iNumVertices = { 0 };

	_uint								m_iIndexStride = { 0 };
	_uint								m_iNumIndices = { 0 };

	DXGI_FORMAT							m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY			m_ePrimitiveTopology;

	D3D11_BUFFER_DESC					m_BufferDesc = {  };
	D3D11_SUBRESOURCE_DATA				m_InitialData = {  };

protected:
	HRESULT Create_Buffer(ID3D11Buffer** ppBuffer);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END