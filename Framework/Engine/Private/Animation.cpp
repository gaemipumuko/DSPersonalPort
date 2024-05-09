#include "..\Public\Animation.h"
#include "Channel.h"
#include "Bone.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_fDuration(rhs.m_fDuration)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_CurrentAnimPosition(rhs.m_CurrentAnimPosition)
	, m_iNumChannels(rhs.m_iNumChannels)
	, m_Channels(rhs.m_Channels)
	, m_bIsFinished(rhs.m_bIsFinished)
	, m_CurrentKeyFrameIndices(rhs.m_CurrentKeyFrameIndices)
{
	strcpy_s(m_szName, rhs.m_szName);

	for (auto& pChannel : m_Channels)
	{
		Safe_AddRef(pChannel);
	}

}


HRESULT CAnimation::Initialize(ANIM_DESC AnimDesc, const vector<class CBone*>& Bones)
{
	strcpy_s(m_szName, AnimDesc.mName);
	m_fDuration = AnimDesc.mDuration;
	m_fTickPerSecond = AnimDesc.mTicksPerSecond;

	m_iNumChannels = AnimDesc.mNumChannels;

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CHANNEL_DESC* mChannel = *AnimDesc.mChannels;
		CChannel* pChannel = CChannel::Create(mChannel[i], Bones);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Invalidate_TransformationMatrix(_bool isLoop, const vector<class CBone*>& Bones, _float fTimeDelta)
{
	/* 현재 내 애니메이션의 재생 위치를 구한다. */
	m_CurrentAnimPosition += m_fTickPerSecond * fTimeDelta;

	if (isLoop == true)
	m_bIsFinished = true;


	if (m_CurrentAnimPosition >= m_fDuration)
	{

		m_bIsFinished = true;

		if (isLoop == true)
		{
			m_CurrentAnimPosition = 0.f;
			//m_bIsFinished = true;
		}

	}

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		m_Channels[i]->Invalidate_TransformationMatrix(Bones, &m_CurrentKeyFrameIndices[i], m_CurrentAnimPosition);
	}



 }

_bool CAnimation::Change_Animation(vector<class CChannel*>& PrevChannels, const vector<class CBone*>& Bones, _float fTimeDelta)
{
	m_CurrentPosition += fTimeDelta;

	if (m_CurrentPosition >= .1f)
	{

		m_CurrentPosition = 0.f;
		m_CurrentAnimPosition = 0.f;

		return false;
	}



	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		m_Channels[i]->Change_Animation(PrevChannels,Bones, m_CurrentPosition);
	}


	return true;
}

void CAnimation::Ready_Animation()
{
	m_bIsFinished = false;
	m_CurrentAnimPosition = 0.f; 
	for (_uint i = 0; i<m_iNumChannels ; i++ )
	{
		m_CurrentKeyFrameIndices[i] = 0;
	}
}

CAnimation * CAnimation::Create(ANIM_DESC AnimDesc, const vector<class CBone*>& Bones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(AnimDesc, Bones)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
