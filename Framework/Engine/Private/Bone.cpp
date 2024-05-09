#include "..\Public\Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(NODE_DESC BoneDesc, _int iParentBoneIndex)
{
	strcpy_s(m_szName, BoneDesc.mName);

	memcpy(&m_TransformationMatrix, &BoneDesc.mTransformation, sizeof(_float4x4));

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombindTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CBone::Invalidate_CombinedTransformationMatrix(const vector<CBone*>& Bones)
{
	if (-1 == m_iParentBoneIndex)
		m_CombindTransformationMatrix = m_TransformationMatrix;

	else
	{
		/* m_CombindTransformationMatrix  = 나만의 상태행렬(TransformationMatrix) * 부모의 최종행렬(Parent`s CombinedMatrix) */
		XMStoreFloat4x4(&m_CombindTransformationMatrix, 
			XMLoadFloat4x4(&m_TransformationMatrix) * 
			XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombindTransformationMatrix));
	}
}

CBone * CBone::Create(NODE_DESC BoneDesc, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(BoneDesc, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	
}
