#pragma once

/* 모델을 구성하는 하나의 파츠. */
/* 정점과 인덱스를 구성하기위해 디자인된 클래스다. */
#include "VIBuffer.h"
#include "Model.h"


BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, MESH_DESC MeshDesc, const vector<class CBone*>& Bones, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_BoneMatrices(CShader* pShader, const vector<class CBone*>& Bones, const _char* pConstantName, _fmatrix PivotMatrix);
	_float	Compare_Height(_fvector vTargetPos);
	_float	Compare_Height_Rev(_fvector vTargetPos);

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = { 0 };
	_uint				m_iNumBones = { 0 };
	vector<_uint>		m_BoneIndices; /* 이 메시에 영향을 주는 뼈들의 인덱스(전체뼈기준)들을 모아둔다. */
	vector<_float4x4>	m_OffsetMatices;

private:
	HRESULT Ready_NonAnim_Mesh(MESH_DESC MeshDesc, _fmatrix PivotMatrix);
	HRESULT Ready_Anim_Mesh(MESH_DESC MeshDesc, const vector<class CBone*>& Bones);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, MESH_DESC MeshDesc, const vector<class CBone*>& Bones, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END