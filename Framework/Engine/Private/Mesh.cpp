#include "..\Public\Mesh.h"
#include "Bone.h"
#include "Shader.h"

CMesh::CMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, MESH_DESC MeshDesc, const vector<class CBone*>& Bones, _fmatrix PivotMatrix)
{
	m_iMaterialIndex = MeshDesc.mMaterialIndex;
	strcpy_s(m_szName, MeshDesc.mName);
	m_iNumVertexBuffers = 1;
	m_iNumVertices = MeshDesc.mNumVertices;
	m_iIndexStride = 4;
	m_iNumIndices = MeshDesc.mNumFaces * 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	HRESULT		hr = eModelType == CModel::TYPE_NONANIM ? Ready_NonAnim_Mesh(MeshDesc, PivotMatrix) : Ready_Anim_Mesh(MeshDesc, Bones);
	if (FAILED(hr))
		return E_FAIL;
	
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

	_ulong*		pIndices = new _ulong[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ulong) * m_iNumIndices);

	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = 0;

	for (size_t i = 0; i < MeshDesc.mNumFaces; ++i)
	{
		FACE_DESC	AIFace = MeshDesc.mFaces[i];	

		m_pIndices[iNumIndices] = AIFace.mIndices[0];
		pIndices[iNumIndices++] = AIFace.mIndices[0];
		m_pIndices[iNumIndices] = AIFace.mIndices[1];
		pIndices[iNumIndices++] = AIFace.mIndices[1];
		m_pIndices[iNumIndices] = AIFace.mIndices[2];
		pIndices[iNumIndices++] = AIFace.mIndices[2];
	}

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CMesh::Bind_BoneMatrices(CShader * pShader, const vector<CBone*>& Bones, const _char * pConstantName, _fmatrix PivotMatrix)
{
	/* vtf : Vertex Texture Fetch */
	/* 메시에게 영향을 주는 뼈들의 행려을 셰이더에 한번에 던져주기위해서 행렬배열을 만들어야한다. */
	_float4x4		BoneMatrices[256] = {};
	ZeroMemory(BoneMatrices, sizeof(_float4x4) * 256);

	_uint			iNumBones = 0;

	for (size_t i = 0; i < m_iNumBones; i++)
	{	
		XMStoreFloat4x4(&BoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatices[i]) * 
			XMLoadFloat4x4(Bones[m_BoneIndices[i]]->Get_CombinedMatrix()) * PivotMatrix);
	}

	return pShader->Bind_Matrices(pConstantName, BoneMatrices, m_iNumBones);
}

_float CMesh::Compare_Height(_fvector vTargetPos)
{

 	_uint	iNumFaces = m_iNumIndices / 3;
	_uint	iIndex = 0;
	_float3 fPt1, fPt2, fPt3;
	_float fDist = 0.f;
	_vector vPlane; 
	_vector vTargetPosition = XMVectorSetY(vTargetPos, 0.f);

	for (size_t i = 0; i < iNumFaces; i++)
	{	
		fPt1 = m_pVerticesPos[m_pIndices[iIndex++]];
		fPt2 = m_pVerticesPos[m_pIndices[iIndex++]];
		fPt3 = m_pVerticesPos[m_pIndices[iIndex++]];

		_vector vPt[3];
		_float3 vPtNormal[3];
		_vector vLine;

		vPt[0] = XMVectorSetY(XMLoadFloat3(&fPt1), 0.f);
		vPt[1] = XMVectorSetY(XMLoadFloat3(&fPt2), 0.f);
		vPt[2] = XMVectorSetY(XMLoadFloat3(&fPt3), 0.f);

		//vLine = XMVector3Normalize(vPt[1] - vPt[0]);
		//vPtNormal[0] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		//vLine = XMVector3Normalize(vPt[2] - vPt[1]);
		//vPtNormal[1] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		//vLine = XMVector3Normalize(vPt[0] - vPt[2]);
		//vPtNormal[2] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMVector3Normalize(vPt[1] - vPt[0]);
		vPtNormal[0] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMVector3Normalize(vPt[2] - vPt[1]);
		vPtNormal[1] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMVector3Normalize(vPt[0] - vPt[2]);
		vPtNormal[2] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		_bool bLine[3] = { true,true,true };
		for (size_t j = 0; j < 3; j++)
		{
			_vector	vDir = vTargetPosition - vPt[j];

			if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir),XMLoadFloat3(&vPtNormal[j]))))
			{
				bLine[j] = false;
				//return XMVectorGetW(vPlane);
				
			}


		}
		if (bLine[0] && bLine[1] && bLine[2])
		{
			vPlane = XMPlaneFromPoints(XMLoadFloat3(&fPt1), XMLoadFloat3(&fPt2), XMLoadFloat3(&fPt3));
			return -(XMVectorGetX(vPlane) * XMVectorGetX(vTargetPos) + XMVectorGetZ(vPlane) * XMVectorGetZ(vTargetPos) + XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

		}
				
		//if (TriangleTests::Intersects(vTargetPos, XMVectorSet(0.f, -1.f, 0.f, 0.f), XMLoadFloat3(&fPt1) , XMLoadFloat3(&fPt2) , XMLoadFloat3(&fPt3), fDist))
		//{
		//	vPlane = XMPlaneFromPoints(XMLoadFloat3(&fPt1), XMLoadFloat3(&fPt2), XMLoadFloat3(&fPt3));

		//	return -(XMVectorGetX(vPlane) * XMVectorGetX(vTargetPos) + XMVectorGetZ(vPlane) * XMVectorGetZ(vTargetPos) + XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
		//}
	}
	

	return 0.f;
}

_float CMesh::Compare_Height_Rev(_fvector vTargetPos)
{
	_uint	iNumFaces = m_iNumIndices / 3;
	_uint	iIndex = 0;
	_float3 fPt1, fPt2, fPt3;
	_float fDist = 0.f;
	_vector vPlane;
	_vector vTargetPosition = XMVectorSetY(vTargetPos, 0.f);

	for (size_t i = 0; i < iNumFaces; i++)
	{
		fPt1 = m_pVerticesPos[m_pIndices[iIndex++]];
		fPt2 = m_pVerticesPos[m_pIndices[iIndex++]];
		fPt3 = m_pVerticesPos[m_pIndices[iIndex++]];

		_vector vPt[3];
		_float3 vPtNormal[3];
		_vector vLine;

		vPt[0] = XMVectorSetY(XMLoadFloat3(&fPt1), 0.f);
		vPt[1] = XMVectorSetY(XMLoadFloat3(&fPt2), 0.f);
		vPt[2] = XMVectorSetY(XMLoadFloat3(&fPt3), 0.f);

		//vLine = XMVector3Normalize(vPt[1] - vPt[0]);
		//vPtNormal[0] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		//vLine = XMVector3Normalize(vPt[2] - vPt[1]);
		//vPtNormal[1] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		//vLine = XMVector3Normalize(vPt[0] - vPt[2]);
		//vPtNormal[2] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMVector3Normalize(vPt[1] - vPt[0]);
		vPtNormal[0] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMVector3Normalize(vPt[2] - vPt[1]);
		vPtNormal[1] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMVector3Normalize(vPt[0] - vPt[2]);
		vPtNormal[2] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		_bool bLine[3] = { true,true,true };
		for (size_t j = 0; j < 3; j++)
		{
			_vector	vDir = vTargetPosition - vPt[j];

			if (0 > XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMLoadFloat3(&vPtNormal[j]))))
			{
				bLine[j] = false;
				//return XMVectorGetW(vPlane);

			}


		}

			if (bLine[0] && bLine[1] && bLine[2])
			{
				vPlane = XMPlaneFromPoints(XMLoadFloat3(&fPt1), XMLoadFloat3(&fPt2), XMLoadFloat3(&fPt3));
				return -(XMVectorGetX(vPlane) * XMVectorGetX(vTargetPos) + XMVectorGetZ(vPlane) * XMVectorGetZ(vTargetPos) + XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

			}
		//if (TriangleTests::Intersects(vTargetPos, XMVectorSet(0.f, -1.f, 0.f, 0.f), XMLoadFloat3(&fPt1) , XMLoadFloat3(&fPt2) , XMLoadFloat3(&fPt3), fDist))
		//{
		//	vPlane = XMPlaneFromPoints(XMLoadFloat3(&fPt1), XMLoadFloat3(&fPt2), XMLoadFloat3(&fPt3));

		//	return -(XMVectorGetX(vPlane) * XMVectorGetX(vTargetPos) + XMVectorGetZ(vPlane) * XMVectorGetZ(vTargetPos) + XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
		//}
	}


	return 0.f;
}

HRESULT CMesh::Ready_NonAnim_Mesh(MESH_DESC MeshDesc, _fmatrix PivotMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);

	VTXMESH*		pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_pVerticesPos[i], &MeshDesc.mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vPosition, &MeshDesc.mVertices[i], sizeof(_float3));
		XMStoreFloat3(&m_pVerticesPos[i], XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &MeshDesc.mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexcoord, &MeshDesc.mTextureCoords[i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &MeshDesc.mTangents[i], sizeof(_float3));
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_Anim_Mesh(MESH_DESC MeshDesc, const vector<class CBone*>& Bones)
{
	m_iVertexStride = sizeof(VTXANIMMESH);


	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);

	VTXANIMMESH*		pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &MeshDesc.mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &MeshDesc.mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &MeshDesc.mTextureCoords[i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &MeshDesc.mTangents[i], sizeof(_float3));
	}	

	/* 메시의 뼈의 갯수(mNumBones). (전체뼈의 갯수( x), 이 메시에게 영향ㅇ르 주는 뼈의 갯수(o) */
	/* vBlendIndices : 이 메시에게 영향ㅇ르 주는 뼈기준의 인덱스들 */
	/* vBlendWeights : */
	m_iNumBones = MeshDesc.mNumBones;

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		BONE_DESC* pAIBone = *MeshDesc.mBones;

		/* 메시에  mOffsetMatrix행렬이 담겨있는 형태다. */
		/* 공통된 뼈 셋트를 서로 다른 메시에 적용해주기위해 필요한 보정 상태를 가지고 있는 행렬. */
		_float4x4		OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone[i].mOffsetMatrix, sizeof(_float4x4));

		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_OffsetMatices.push_back(OffsetMatrix);

		_uint		iBoneIndex = 0;

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool {					
			if (0 == strcmp(pBone->Get_BoneName(), pAIBone[i].mName))
				return true;
			++iBoneIndex;
			return false;
		});	

		m_BoneIndices.push_back(iBoneIndex);

		for (size_t j = 0; j < pAIBone[i].mNumWeights; j++)
		{

			if (0.0f == pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.x)
			{
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendIndices.x = i;
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.x = pAIBone[i].mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.y)
			{
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendIndices.y = i;
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.y = pAIBone[i].mWeights[j].mWeight;
			}
			else if (0.0f == pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.z)
			{
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendIndices.z = i;
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.z = pAIBone[i].mWeights[j].mWeight;
			}
			else if (0.0f == pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.w)
			{
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendIndices.w = i;
				pVertices[pAIBone[i].mWeights[j].mVertexId].vBlendWeights.w = pAIBone[i].mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		_uint		iBoneIndex = 0;

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool {
			if (0 == strcmp(pBone->Get_BoneName(), m_szName))
				return true;
			++iBoneIndex;
			return false;
		});

		_float4x4		OffsetMatrix;

		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());	

		m_OffsetMatices.push_back(OffsetMatrix);

		m_BoneIndices.push_back(iBoneIndex);
	}


	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, MESH_DESC MeshDesc, const vector<class CBone*>& Bones, _fmatrix PivotMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, MeshDesc, Bones, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMesh::Clone(void * pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();

}
