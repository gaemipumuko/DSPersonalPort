#pragma once

/* � ���̵� �ټ��� �޽÷� �����Ǿ� �ִ�. */
/* �� = �޽� + �޽� + �޽�... */
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
	/* .fbx���Ϸκ��� �о�� ������ 1�������� �����Ѵ�.*/
	Assimp::Importer			m_Importer;

	// m_pAIScene = m_Importer.ReadFile( ���, �÷��� );
	const aiScene*				m_pAIScene = { nullptr };

private:
	_bool						m_bIsPlaying = { false };
	_bool						m_bChangeAnimation = { false };
	_bool						m_bIsRev = { true };

	TYPE						m_eModelType = { TYPE_END };
	_float4x4					m_PivotMatrix;

	/* ���� �������� �޽÷� �����Ǿ��ִ�.*/
	/* �� == �޽�(VB, IB) + �޽�(VB, IB) + �޽�(VB, IB)*/
	/* �޽ð�ü�鸦 �����.(������ �ε������۸� �����Ѵ�. )*/
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

	/* 1�������� ����� �����ͷκ��� ���� ����� ������ �����ͼ� ���� ����ϱ� ���ϵ��� �� ��Ÿ�ϴ�� �����Ѵ�. */
private:	
	HRESULT Ready_Meshes(HANDLE hFile);
	/* ������ �ѵ��Ѵ�. == �ȼ����� ������ �ο��Ǿ��־���Ѵ�. == �ؽ���. */
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

