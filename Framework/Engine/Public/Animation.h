#pragma once

#include "Base.h"

/* 특정 애니메이션을 구동하기위한 정보를 가진다. */
/* 전체시간(Duration), 속도(TickPerSecond), 제어하는 뼈의 정보들. */


BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(ANIM_DESC AnimDesc, const vector<class CBone*>& Bones);
	void Invalidate_TransformationMatrix(_bool isLoop, const vector<class CBone*>& Bones, _float fTimeDelta);
	vector<class CChannel*>* Get_CurrentChannels() { return &m_Channels; }
	_bool Change_Animation(vector<class CChannel*>& PrevChannels, const vector<class CBone*>& Bones, _float fTimeDelta);
	_bool Change_Animation(const vector<class CBone*>& PrevBones, const vector<class CBone*>& Bones, _float fTimeDelta);
	_bool Get_Finished() { return m_bIsFinished; }
	void Ready_Animation();
private:
	_char						m_szName[MAX_PATH] = "";
	_float						m_fDuration = { 0.f };
	_float						m_fTickPerSecond = { 0.f };
	_float						m_CurrentAnimPosition = { 0.f };
	_float						m_CurrentPosition = { 0.f };

	_uint						m_iNumChannels = { 0 };
	vector<class CChannel*>		m_Channels;
	_bool						m_bIsFinished = { false };
	vector<_uint>				m_CurrentKeyFrameIndices;

	

public:
	static CAnimation* Create(ANIM_DESC AnimDesc, const vector<class CBone*>& Bones);
	CAnimation* Clone();
	virtual void Free() override;
};

END