#pragma once

#include "Base.h"

/* Ư�� �ִϸ��̼ǿ��� �����ϴ� ���� ����. */
/* �ִϸ��̼��� ��ü ����Ǵ� ���ȿ� ������ �ð��뿡 ���� �� ���� ���µ鸦 �����Ѵ�. */

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

	/* ���� �� ���� �� �ִϸ��̼��� ����Ҷ� � Ű�����ӱ��� �����Ͽ� ���¸� ǥ���ϰ� �־��°�?! */



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