#pragma once

#include "Base.h"

/* 특정 애니메이션에서 구동하는 뼈의 정보. */
/* 애니메이션이 전체 재생되는 동안에 각각의 시간대에 따른 이 뼈의 상태들를 저장한다. */

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(CHANNEL_DESC ChannelDesc, const vector<class CBone*>& Bones);
	void Invalidate_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex, _float fCurrentAnimPosition);
	void Change_Animation(vector<CChannel*>& PrevChannels, const vector<class CBone*>& Bones, _float fDeltaTime);
	const _uint Get_BoneIndex() { return m_iBoneIndex; }
	const KEYFRAME Get_KeyFrame() { return m_KeyFrames.front(); }
	const KEYFRAME Get_Current_KeyFrame() { return m_CurrentKeyFrame; }

private:
	_char					m_szName[MAX_PATH] = "";

	/* 현재 내 뼈는 이 애니메이션을 재생할때 어떤 키프레임까지 증가하여 상태를 표현하고 있었는가?! */



	_uint					m_iCurrentKeyFrame = { 0 };
	_uint					m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>		m_KeyFrames;
	_uint					m_iBoneIndex = { 0 };

	KEYFRAME				m_CurrentKeyFrame = {};

public:
	static CChannel* Create(CHANNEL_DESC ChannelDesc, const vector<class CBone*>& Bones);
	virtual void Free() override;
};

END