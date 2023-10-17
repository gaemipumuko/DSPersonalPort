#pragma once

/* 카메라월드위치 뷰, 투영 행렬을 보관하낟. */
/* 필요 시, 카메라위치, 정, 역행렬 리턴한다. */
/* Tick() : 카메라 위치도 구해놓느다. 파이프라인이 들ㅇ고 있는 뷰, 투영행령을 역행렬을 한번 구해놓느다. */
#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	_float4 Get_CamPosition() const {
		return m_vCamPosition;
	}

	_float4x4 Get_Transform_Float4x4(TRANSFORMSTATE eTransformState) const {
		return m_TransformMatrix[eTransformState];
	}

	_float4x4 Get_Transform_Inverse_Float4x4(TRANSFORMSTATE eTransformState) const {
		return m_TransformMatrix_Inverse[eTransformState];
	}

	_matrix Get_Transform(TRANSFORMSTATE eTransformState) const {
		return XMLoadFloat4x4(&m_TransformMatrix[eTransformState]);
	}

	_matrix Get_Transform_Inverse(TRANSFORMSTATE eTransformState) const {
		return XMLoadFloat4x4(&m_TransformMatrix_Inverse[eTransformState]);
	}

public:
	void Set_Transform(TRANSFORMSTATE eTransformState, const _float4x4& TransformMatrix) {
		m_TransformMatrix[eTransformState] = TransformMatrix;
	}

	void Set_Transform(TRANSFORMSTATE eTransformState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrix[eTransformState], TransformMatrix);
	}

	

public:
	HRESULT Initialize();
	void Tick();

private:
	_float4x4			m_TransformMatrix[D3DTS_END];
	_float4x4			m_TransformMatrix_Inverse[D3DTS_END];
	_float4				m_vCamPosition;


public:
	static CPipeLine* Create();
	virtual void Free() override;
};

END