#include "..\Public\Model.h"
#include "Bone.h"
#include "Mesh.h"
#include "Texture.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
	, m_eModelType(rhs.m_eModelType)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_PrevTransformationMatrices(rhs.m_PrevTransformationMatrices)
{
	for (auto& pPrototypeAnimation : rhs.m_Animations)
	{
		m_Animations.push_back(pPrototypeAnimation->Clone());
	}


	for (auto& pPrototypeBone : rhs.m_Bones)
	{
		m_Bones.push_back(pPrototypeBone->Clone());
	}

	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		{
			Safe_AddRef(Material.pMaterials[i]);
		}
	}

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);	
}

void CModel::Set_Animation(_uint iAnimIndex, _bool bIsLoop)
{
	m_iPrevAnimIndex = m_iCurrentAnimIndex;
	m_iCurrentAnimIndex = iAnimIndex;
	if ((m_iPrevAnimIndex != m_iCurrentAnimIndex))
	{
	m_Animations[m_iCurrentAnimIndex]->Ready_Animation();
		//if(bIsLoop == false)

	m_bChangeAnimation = true;

	for (size_t i = 0; i < m_Bones.size(); i++)
	{
		m_PrevTransformationMatrices[i] = m_Bones[i]->Get_TransformationMatrix();

	}

	}

}

const _float4x4* CModel::Get_BoneCombinedMatrix(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (0 == strcmp(pBone->Get_BoneName(), pBoneName))
			return true;
		return false;
		});

	if (iter == m_Bones.end())
	{
		_float4x4 matidentity;
		XMStoreFloat4x4(&matidentity, (XMMatrixIdentity()));
		return &matidentity;
	}

	return (*iter)->Get_CombinedMatrix();
}

void CModel::Bind_Parts(vector<class CBone*>* pParentBones)
{
	for(auto pBones : *pParentBones)
	{
		const _char* pBoneName;
		pBoneName = pBones->Get_BoneName();
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (0 == strcmp(pBone->Get_BoneName(), pBoneName))
			return true;
		return false;
		});

	if (iter == m_Bones.end())
	{	
		continue;
	}
	else
	{
	(*iter)->Set_TransformationMatrix(XMLoadFloat4x4(&(pBones->Get_TransformationMatrix())));

	}

	}
	for (auto& pBone : m_Bones)
	{
		pBone->Invalidate_CombinedTransformationMatrix(m_Bones);
	}

}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const string& strModelFilePath, _fmatrix PivotMatrix)
{
	m_eModelType = eModelType;

	/* aiProcess_PreTransformVertices : */
	/* 이 모델을 구성하는 메시들이 혹여 지정된 상태대로 그려져야한다면. 그 지정된 상태행렬응ㄹ 이 메시의 정점에 미리 곱해서 로드한다. */
	_uint		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (TYPE_NONANIM == eModelType)
		iFlag |= aiProcess_PreTransformVertices;


	_ulong dwByte;

	wstring strModelPath(strModelFilePath.begin(), strModelFilePath.end());

	HANDLE		hFile = CreateFile(strModelPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;


	_uint iFlags, iNumAnimations, iNumMaterials, iNumMeshes, iNumTextures;

	ReadFile(hFile, &iFlags, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &iNumAnimations, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &iNumMaterials, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &iNumMeshes, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);

	m_iNumAnimations = iNumAnimations;
	m_iNumMaterials = iNumMaterials;
	m_iNumMeshes = iNumMeshes;
	m_iNumTextures = iNumTextures;


	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);


	/* mRootNode : 모든 뼈의 가장 시작이되는 뼈다. */
	if (FAILED(Ready_Bones(-1,hFile)))
		return E_FAIL;

	for (size_t i = 0; i < m_Bones.size(); i++)
	{
		m_PrevTransformationMatrices.push_back(m_Bones[i]->Get_TransformationMatrix());

	}

	if (FAILED(Ready_Meshes(hFile)))
		return E_FAIL;

	if (FAILED(Ready_Materials(strModelFilePath, hFile)))
		return E_FAIL;

	if (FAILED(Ready_Animations(hFile)))
		return E_FAIL;
	
	CloseHandle(hFile);
	
	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

_bool CModel::Play_Animation(_bool isLoop, _float fTimeDelta)
{
	if (m_bChangeAnimation == true)
	{
		Linear_Interpolation(isLoop, fTimeDelta);
	}
	else
	{
	m_Animations[m_iCurrentAnimIndex]->Invalidate_TransformationMatrix(isLoop, m_Bones, fTimeDelta);
	}


	/* 모든 뼈들의 CombinedTransformationMatrix를 갱신한다. */
	for (auto& pBone : m_Bones)
	{
		pBone->Invalidate_CombinedTransformationMatrix(m_Bones);
	}

	return m_Animations[m_iCurrentAnimIndex]->Get_Finished();
}

_bool CModel::Change_Animation(vector<class CChannel*>& PrevChannels, _float fTimeDelta, _uint iAnimIndex)
{
	m_CurrentTime += fTimeDelta;

	m_Animations[m_iCurrentAnimIndex]->Change_Animation(PrevChannels, m_Bones, fTimeDelta);

	for (auto& pBone : m_Bones)
	{
		pBone->Invalidate_CombinedTransformationMatrix(m_Bones);
	}

	if (m_CurrentTime >= 0.1f)
	{
		m_CurrentTime = 0.f;
		return true;
	}
	else
		return false;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	/* iMeshIndex에 해당하는 메시에 영향을 주는 뼈들을 모아서 셰이더로 전달한다. */


	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Bind_BoneMatrices(_uint iMeshIndex, CShader * pShader, const _char * pConstantName)
{
	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader, m_Bones, pConstantName, XMLoadFloat4x4(&m_PivotMatrix));	
}

HRESULT CModel::Bind_Material(CShader* pShader, const _char * pConstantName, _uint iMeshIndex, aiTextureType eMaterialType)
{
	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pMaterials[eMaterialType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

_float CModel::Compute_Height(_fvector vTargetWorldPos, CTransform* pOwnerTransform, _bool isRev)
{
	_vector			vTargetPos = XMVector3TransformCoord(vTargetWorldPos, pOwnerTransform->Get_WorldMatrix_Inverse());
	_float			fTarget = {};
	_float			fHeight = {-100.f};

	m_bIsRev = isRev;

		for (size_t i = 0; i < m_iNumMeshes; i++)
		{

			if (m_bIsRev == false)
			{
				fTarget = m_Meshes[i]->Compare_Height(vTargetPos);

			}
			else
			{
				fTarget = m_Meshes[i]->Compare_Height_Rev(vTargetPos);
			}


			if (fTarget != 0.f && (fTarget > fHeight))
			{

				fHeight = fTarget;
			}

		}
	
	//fTarget = m_Meshes[0]->Compare_Height(vTargetPos);

	return fHeight;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const string& strModelFilePath, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, strModelFilePath, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


HRESULT CModel::Ready_Meshes(HANDLE hFile)
{
	_ulong dwByte;

	m_Meshes.reserve(m_iNumMeshes);

		for (size_t i = 0; i < m_iNumMeshes; i++)
		{
		MESH_DESC MeshDesc = {};
		BONE_DESC* pBoneDesc;
		VERTEX_WEIGHTS* pWeights;
		FACE_DESC* pFaces;

		ReadFile(hFile, &MeshDesc.mMaterialIndex, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &MeshDesc.mName, sizeof(_char) * 1024, &dwByte, nullptr);
		ReadFile(hFile, &MeshDesc.mNumVertices, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &MeshDesc.mNumBones, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &MeshDesc.mNumFaces, sizeof(_uint), &dwByte, nullptr);

		MeshDesc.mVertices = new XMFLOAT3[MeshDesc.mNumVertices];
		MeshDesc.mNormals = new XMFLOAT3[MeshDesc.mNumVertices];
		MeshDesc.mTextureCoords = new XMFLOAT2[MeshDesc.mNumVertices];
		MeshDesc.mTangents = new XMFLOAT3[MeshDesc.mNumVertices];
		pBoneDesc = new BONE_DESC[MeshDesc.mNumBones];
		pFaces = new FACE_DESC[MeshDesc.mNumFaces];
		MeshDesc.mBones = &pBoneDesc;
		for (size_t j = 0; j < MeshDesc.mNumVertices; j++)
		{
			ReadFile(hFile, &MeshDesc.mVertices[j], sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &MeshDesc.mNormals[j], sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &MeshDesc.mTextureCoords[j], sizeof(_float2), &dwByte, nullptr);
			ReadFile(hFile, &MeshDesc.mTangents[j], sizeof(_float3), &dwByte, nullptr);
		}

		for (size_t j = 0; j < MeshDesc.mNumBones; j++)
		{
			ReadFile(hFile, &pBoneDesc[j].mOffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
			ReadFile(hFile, &pBoneDesc[j].mName, sizeof(_char) * 1024, &dwByte, nullptr);
			ReadFile(hFile, &pBoneDesc[j].mNumWeights, sizeof(_uint), &dwByte, nullptr);
			pBoneDesc[j].mWeights = new VERTEX_WEIGHTS[pBoneDesc[j].mNumWeights];

			for (size_t k = 0; k < pBoneDesc[j].mNumWeights; k++)
			{
				ReadFile(hFile, &pBoneDesc[j].mWeights[k].mVertexId, sizeof(_uint), &dwByte, nullptr);
				ReadFile(hFile, &pBoneDesc[j].mWeights[k].mWeight, sizeof(_float), &dwByte, nullptr);

			}

		}

		for (size_t j = 0; j < MeshDesc.mNumFaces; ++j)
		{

			_uint* mIndices = new _uint[3];
			pFaces[j].mIndices = mIndices;

			ReadFile(hFile, &pFaces[j].mIndices[0], sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &pFaces[j].mIndices[1], sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &pFaces[j].mIndices[2], sizeof(_uint), &dwByte, nullptr);

		}
		MeshDesc.mFaces = pFaces;
		/* VB, IB를 만든다. */
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, MeshDesc, m_Bones, XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);



		for (size_t j = 0; j < MeshDesc.mNumBones; j++)
		{
			Safe_Delete_Array(pBoneDesc[j].mWeights);
		}

		Safe_Delete_Array(*MeshDesc.mBones);

		for (size_t j = 0; j < MeshDesc.mNumFaces; ++j)
		{
			Safe_Delete_Array(MeshDesc.mFaces[j].mIndices);
		}
		Safe_Delete_Array(pFaces);
		Safe_Delete_Array(MeshDesc.mVertices);
		Safe_Delete_Array(MeshDesc.mNormals);
		Safe_Delete_Array(MeshDesc.mTextureCoords);
		Safe_Delete_Array(MeshDesc.mTangents);

	}


	return S_OK;
}

HRESULT CModel::Ready_Materials(const string& strModelFilePath, HANDLE hFile)
{
	/* aiTextureType : 재질 세트(디퓨즈, 스펙큘러, 앰피언트, 노멀, 기타 빛연산에 필요한 여러가지 데이터) 의 갯수. */	
	_ulong dwByte;

	_char		szDrive[MAX_PATH] = "";
	_char		szDirectory[MAX_PATH] = "";
	_char		szFileName[MAX_PATH] = "";
	_char		szExt[MAX_PATH] = "";

	_char		szTextureFilePath[MAX_PATH] = "";

	_splitpath_s(strModelFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	m_Materials.reserve(m_iNumMaterials);
	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MODEL_MATERIAL			ModelMaterials = {};

		m_Materials.push_back(ModelMaterials);
	}


	//Texture 갯수랑 j 값 저장해서 불러올때 사용
	while (true)
	{

		_int			iMaterialIndex = {};
		_uint			iTextureType = {};
		_tchar			szFullPath[MAX_PATH] = L"";
		ReadFile(hFile, &iMaterialIndex, sizeof(_int), &dwByte, nullptr);
		if (iMaterialIndex == -1)
			return S_OK;
		ReadFile(hFile, &iTextureType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &szFullPath, sizeof(_tchar) * 260, &dwByte, nullptr);


		for (size_t i = 0; i < m_iNumMaterials; i++)
		{

			for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			{

				if (i == iMaterialIndex && j == iTextureType)
				{
					m_Materials[i].pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szFullPath);
					if (nullptr == m_Materials[i].pMaterials[j])
						return E_FAIL;
				}

			}

		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones( _int iParentBoneIndex, HANDLE hFile)
{
	_ulong dwByte;
	//자식의 수
	NODE_DESC NodeDesc = {};

	ReadFile(hFile, &NodeDesc.mNumChildren, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &NodeDesc.mName, sizeof(_char) * 1024, &dwByte, nullptr);
	ReadFile(hFile, &NodeDesc.mTransformation, sizeof(_float4x4), &dwByte, nullptr);
	ReadFile(hFile, &NodeDesc.mNumMeshes, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &NodeDesc.mMeshes, sizeof(_uint), &dwByte, nullptr);

	CBone*		pBone = CBone::Create(NodeDesc, iParentBoneIndex);
	if (nullptr == pBone)
		return E_FAIL;
			
	m_Bones.push_back(pBone);

	_int iParentIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < NodeDesc.mNumChildren; i++)
	{
		Ready_Bones(iParentIndex, hFile);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations(HANDLE hFile)
{
	_ulong dwByte;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		ANIM_DESC AnimDesc = {};


		ReadFile(hFile, &AnimDesc.mName, sizeof(_char) * 1024, &dwByte, nullptr);
		ReadFile(hFile, &AnimDesc.mDuration, sizeof(double), &dwByte, nullptr);
		ReadFile(hFile, &AnimDesc.mTicksPerSecond, sizeof(double), &dwByte, nullptr);
		ReadFile(hFile, &AnimDesc.mNumChannels, sizeof(_uint), &dwByte, nullptr);

		CHANNEL_DESC* ChannelDesc = new CHANNEL_DESC[AnimDesc.mNumChannels];

		for (size_t j = 0; j < AnimDesc.mNumChannels; j++)
		{
			ReadFile(hFile, &ChannelDesc[j].mNodeName, sizeof(_char) * 1024, &dwByte, nullptr);
			ReadFile(hFile, &ChannelDesc[j].mNumScalingKeys, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &ChannelDesc[j].mNumRotationKeys, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &ChannelDesc[j].mNumPositionKeys, sizeof(_uint), &dwByte, nullptr);
			ChannelDesc[j].mScalingKeys = new SCALING_KEY[ChannelDesc[j].mNumScalingKeys];
			ChannelDesc[j].mRotationKeys = new ROTATION_KEY[ChannelDesc[j].mNumRotationKeys];
			ChannelDesc[j].mPositionKeys = new POSITION_KEY[ChannelDesc[j].mNumPositionKeys];

			for (size_t k = 0; k < ChannelDesc[j].mNumScalingKeys; k++)
			{
				ReadFile(hFile, &ChannelDesc[j].mScalingKeys[k].mValue, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &ChannelDesc[j].mScalingKeys[k].mTime, sizeof(double), &dwByte, nullptr);

			}
			for (size_t k = 0; k < ChannelDesc[j].mNumRotationKeys; k++)
			{
				ReadFile(hFile, &ChannelDesc[j].mRotationKeys[k].mValue.x, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &ChannelDesc[j].mRotationKeys[k].mValue.y, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &ChannelDesc[j].mRotationKeys[k].mValue.z, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &ChannelDesc[j].mRotationKeys[k].mValue.w, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &ChannelDesc[j].mRotationKeys[k].mTime, sizeof(double), &dwByte, nullptr);
			}
			for (size_t k = 0; k < ChannelDesc[j].mNumPositionKeys; k++)
			{
				ReadFile(hFile, &ChannelDesc[j].mPositionKeys[k].mValue, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &ChannelDesc[j].mPositionKeys[k].mTime, sizeof(double), &dwByte, nullptr);
			}

		}
		AnimDesc.mChannels = &ChannelDesc;

		CAnimation*		pAnimation = CAnimation::Create(AnimDesc, m_Bones);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);

		for (size_t i = 0; i < AnimDesc.mNumChannels; i++)
		{
			Safe_Delete_Array(ChannelDesc[i].mScalingKeys);
			Safe_Delete_Array(ChannelDesc[i].mRotationKeys);
			Safe_Delete_Array(ChannelDesc[i].mPositionKeys);
		}
		Safe_Delete_Array(ChannelDesc);

	}

	return S_OK;
}

void CModel::Linear_Interpolation(_bool isLoop, _float fTimeDelta)
{
	m_Animations[m_iCurrentAnimIndex]->Invalidate_TransformationMatrix(isLoop, m_Bones, fTimeDelta);

	for (auto& pBone : m_Bones)
	{
		pBone->Invalidate_CombinedTransformationMatrix(m_Bones);
	}

	//Bones need to be Invalidated to Resulting Animations First Frame Before Interpolated

	m_CurrentPosition += fTimeDelta;

	if (m_CurrentPosition >= .1f)
	{

		m_bChangeAnimation = false;
		m_CurrentPosition = 0.f;

		return;
	}


	_float		fRatio = m_CurrentPosition / .1f;

	_vector		vAnimScale;
	_vector		vAnimRotation;
	_vector		vAnimTranslation;

	_vector		vSourScale, vDestScale;
	_vector		vSourRotation, vDestRotation;
	_vector		vSourTranslation, vDestTranslation;

	_bool		bChannelAvail = { false };


	for (size_t i = 0; i < m_Bones.size(); i++)
	{
		XMMatrixDecompose(&vSourScale, &vSourRotation, &vSourTranslation, XMLoadFloat4x4(&m_PrevTransformationMatrices[i]));
		XMMatrixDecompose(&vDestScale, &vDestRotation, &vDestTranslation, XMLoadFloat4x4(&m_Bones[i]->Get_TransformationMatrix()));

		vAnimScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vAnimRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vAnimTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);

		_matrix			TransformationMatrix = XMMatrixAffineTransformation(vAnimScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vAnimRotation, vAnimTranslation);

		m_Bones[i]->Set_TransformationMatrix(TransformationMatrix);
	}


}

CComponent * CModel::Clone(void * pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		{
			Safe_Release(Material.pMaterials[i]);
		}
	}

	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Importer.FreeScene();

}
