#include "..\Public\Channel.h"
#include "Bone.h"

CChannel::CChannel()
{

}

HRESULT CChannel::Initialize(CHANNEL_DESC ChannelDesc, const vector<class CBone*>& Bones)
{
	strcpy_s(m_szName, ChannelDesc.mNodeName);

	_uint		iBoneIndex = 0;

	auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool {
		if (0 == strcmp(pBone->Get_BoneName(), m_szName))
			return true;
		++iBoneIndex;
		return false;
	});

	m_iBoneIndex = iBoneIndex;


	m_iNumKeyFrames = max(ChannelDesc.mNumScalingKeys, ChannelDesc.mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, ChannelDesc.mNumPositionKeys);

	KEYFRAME			KeyFrame = {};

	_float4				vScale = {}, vRotation = {}, vTranslation = {0.0f, 0.f, 0.f, 1.f};

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		if (i < ChannelDesc.mNumScalingKeys)
		{
			memcpy(&vScale, &ChannelDesc.mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = ChannelDesc.mScalingKeys[i].mTime;
		}

		if (i < ChannelDesc.mNumRotationKeys)
		{
			/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
			vRotation.x = ChannelDesc.mRotationKeys[i].mValue.x;
			vRotation.y = ChannelDesc.mRotationKeys[i].mValue.y;
			vRotation.z = ChannelDesc.mRotationKeys[i].mValue.z;
			vRotation.w = ChannelDesc.mRotationKeys[i].mValue.w;

			KeyFrame.fTime = ChannelDesc.mRotationKeys[i].mTime;
		}

		if (i < ChannelDesc.mNumPositionKeys)
		{
			memcpy(&vTranslation, &ChannelDesc.mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = ChannelDesc.mPositionKeys[i].mTime;
		}
		
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vTranslation = vTranslation;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex, _float fCurrentAnimPosition)
{
	if (0.0f == fCurrentAnimPosition)
		*pCurrentKeyFrameIndex = 0;
	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_vector			vScale;
	_vector			vRotation;
	_vector			vTranslation;

	if (fCurrentAnimPosition >= LastKeyFrame.fTime)
	{
		vScale = XMLoadFloat4(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMLoadFloat4(&LastKeyFrame.vTranslation);
	}
	else
	{

		// 프레임이 드랍될 경우 키프레임이 넘어갈 수 있으므로 
		while (fCurrentAnimPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTime)
			++*pCurrentKeyFrameIndex;

		_float		fRatio = (fCurrentAnimPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTime) /
			(m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTime - m_KeyFrames[*pCurrentKeyFrameIndex].fTime);


		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourTranslation, vDestTranslation;

		vSourScale = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale);
		vDestScale = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale);
		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);

		vSourRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);

		vSourTranslation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation);
		vDestTranslation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation);
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);	
	}

	m_CurrentKeyFrame.fTime = fCurrentAnimPosition;
	XMStoreFloat4(&m_CurrentKeyFrame.vScale,vScale);
	XMStoreFloat4(&m_CurrentKeyFrame.vRotation, vRotation);
	XMStoreFloat4(&m_CurrentKeyFrame.vTranslation, vTranslation);
	//흐른시간/ 0.2 가 변경 ratio 대신

	//XMMatrixScaling();

	//XMMatrixRotationQuaternion();

	//XMMatrixTranslation();

	//XMQuaternionRotationMatrix()


	//선형매핑방법 affinetransformation
	_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

}

void CChannel::Change_Animation(vector<CChannel*>& PrevChannels, const vector<class CBone*>& Bones, _float fDeltaTime)
{

	_float		fRatio = fDeltaTime / .1f;

	_vector		vAnimScale;
	_vector		vAnimRotation;
	_vector		vAnimTranslation;

	_vector		vSourScale, vDestScale;
	_vector		vSourRotation, vDestRotation;
	_vector		vSourTranslation, vDestTranslation;

	_bool		bChannelAvail = { false };

	auto	iter = find_if(PrevChannels.begin(), PrevChannels.end(), [&](CChannel* pChannel)->_bool {
		if (m_iBoneIndex == pChannel->Get_BoneIndex())
		{
			vDestScale = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
			vSourScale = XMLoadFloat4(&pChannel->Get_Current_KeyFrame().vScale);
			vAnimScale = XMVectorLerp(vSourScale, vDestScale, fRatio);

			vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
			vSourRotation = XMLoadFloat4(&pChannel->Get_Current_KeyFrame().vRotation);
			vAnimRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);

			vDestTranslation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vTranslation);
			vSourTranslation = XMLoadFloat4(&pChannel->Get_Current_KeyFrame().vTranslation);
			vAnimTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);

			_matrix			TransformationMatrix = XMMatrixAffineTransformation(vAnimScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vAnimRotation, vAnimTranslation);

			Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

			//vSourScale = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
			//vDestScale = XMLoadFloat4(&pChannel->Get_KeyFrame().vScale);
			//vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);

			//vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
			//vDestRotation = XMLoadFloat4(&pChannel->Get_KeyFrame().vRotation);
			//vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);

			//vSourTranslation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vTranslation);
			//vDestTranslation = XMLoadFloat4(&pChannel->Get_KeyFrame().vTranslation);
			//vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);

			//_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

			//Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

			bChannelAvail = true;
			return true;
		}

		return false;
	});

	if (bChannelAvail == false)
	{

	XMMatrixDecompose(&vAnimScale,&vAnimRotation,&vAnimTranslation,XMLoadFloat4x4(&Bones[m_iBoneIndex]->Get_TransformationMatrix()));

	
	fRatio = (fDeltaTime * fDeltaTime / .1f) - fDeltaTime;
	if (fRatio < 0)
	{
		fRatio = fDeltaTime / .1f;
	}

	vSourScale = vAnimScale;
	vDestScale = XMLoadFloat4(&m_KeyFrames[0].vScale);
	vAnimScale = XMVectorLerp(vSourScale, vDestScale, fRatio);

	vSourRotation = vAnimRotation;
	vDestRotation = XMLoadFloat4(&m_KeyFrames[0].vRotation);
	vAnimRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);

	vSourTranslation = vAnimTranslation;
	vDestTranslation = XMLoadFloat4(&m_KeyFrames[0].vTranslation);
	vAnimTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);

	_matrix			TransformationMatrix = XMMatrixAffineTransformation(vAnimScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vAnimRotation, vAnimTranslation);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
	}

}

CChannel * CChannel::Create(CHANNEL_DESC ChannelDesc, const vector<class CBone*>& Bones)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(ChannelDesc, Bones)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	m_KeyFrames.clear();
}
