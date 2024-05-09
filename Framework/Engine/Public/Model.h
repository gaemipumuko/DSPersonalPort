#pragma once

/* 어떤 모델이든 다수의 메시로 구성되어 있다. */
/* 모델 = 메시 + 메시 + 메시... */
#include "Component.h"
#include "Animation.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END};
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_float4x4 Get_PivotMatrix() const {
		return m_PivotMatrix;
	}

public:
	void Set_Animation(_uint iAnimIndex, _bool bisLoop);

public:
	const _float4x4* Get_BoneCombinedMatrix(const _char* pBoneName) const;
	void Bind_Parts(vector<class CBone*>* pParentBones);
	virtual HRESULT Initialize_Prototype(TYPE eModelType, const string& strModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	_bool Play_Animation(_bool isLoop, _float fTimeDelta);
	_bool Change_Animation(vector<class CChannel*>& PrevChannels, _float fTimeDelta, _uint iAnimIndex);
	vector<class CChannel*>* Get_CurrentChannels() { return m_Animations[m_iCurrentAnimIndex]->Get_CurrentChannels(); }
	vector<class CBone*>* Get_ModelBones() { return &m_Bones;}
	HRESULT Render(_uint iMeshIndex);

public:
	HRESULT Bind_BoneMatrices(_uint iMeshIndex, class CShader* pShader, const _char* pConstantName);
	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eMaterialType);

public:
	_float Compute_Height(_fvector vTargetWorldPos, class CTransform* pOwnerTransform, _bool isRev = true);

private:
	/* .fbx파일로부터 읽어온 정보를 1차적으로 저장한다.*/
	Assimp::Importer			m_Importer;

	// m_pAIScene = m_Importer.ReadFile( 경로, 플래그 );
	const aiScene*				m_pAIScene = { nullptr };

private:
	_bool						m_bIsPlaying = { false };
	_bool						m_bChangeAnimation = { false };
	_bool						m_bIsRev = { true };

	TYPE						m_eModelType = { TYPE_END };
	_float4x4					m_PivotMatrix;

	/* 모델은 여러개의 메시로 구성되어있다.*/
	/* 모델 == 메시(VB, IB) + 메시(VB, IB) + 메시(VB, IB)*/
	/* 메시객체들를 만든다.(정점과 인덱스버퍼를 생성한다. )*/
	_uint						m_iNumMeshes = { 0 };
	vector<class CMesh*>		m_Meshes;

	_uint						m_iNumMaterials = { 0 };
	vector<MODEL_MATERIAL>		m_Materials;

	vector<class CBone*>			m_Bones;
	vector<_float4x4>				m_PrevTransformationMatrices;
	vector<class CAnimation*>	m_Animations;

	_uint						m_iNumAnimations = { 0 };
	_uint						m_iCurrentAnimIndex = { 0 };
	_uint						m_iPrevAnimIndex = { 0 };



	_uint						m_iNumTextures;

	_float						m_CurrentTime = {};
	_float						m_CurrentPosition = { 0.f };

	/* 1차적으로 저장된 데이터로부터 내가 사용할 정보를 꺼내와서 추후 사용하기 편하도록 내 스타일대로 정리한다. */
private:	
	HRESULT Ready_Meshes(HANDLE hFile);
	/* 재질을 롤드한다. == 픽셀마다 재질이 부여되어있어야한다. == 텍스쳐. */
	HRESULT Ready_Materials(const string& strModelFilePath, HANDLE hFile);
	HRESULT Ready_Bones(_int iParentBoneIndex, HANDLE hFile);
	HRESULT Ready_Animations(HANDLE hFile);
	void	Linear_Interpolation(_bool isLoop, _float fTimeDelta);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const string& strModelFilePath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	
};

END

