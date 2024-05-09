#pragma once

namespace Engine
{
	typedef struct tagGraphic_Desc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND			hWnd;
		unsigned int	iWinSizeX, iWinSizeY;
		WINMODE			isWindowed;

	}GRAPHIC_DESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END};

		TYPE		eLightType = TYPE_END;
		XMFLOAT4	vDirection;
		XMFLOAT4	vPosition;
		float		fRange;

		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;		
	}LIGHT_DESC;

	typedef struct tagModelMaterial
	{
		class CTexture* pMaterials[AI_TEXTURE_TYPE_MAX];
	}MODEL_MATERIAL;

	typedef struct tagKeyFrame
	{
		float			fTime;
		XMFLOAT4		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT4		vTranslation;
	}KEYFRAME;

	typedef struct ENGINE_DLL tagVertex_Position
	{
		XMFLOAT3		vPosition;

		static const unsigned int iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;		

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];
	}VTXPOINT;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Cube_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXMESH;
	
	typedef struct ENGINE_DLL tagVertex_Anim_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;

		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXANIMMESH;

	typedef struct ENGINE_DLL tagVertex_Instance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vPosition;
	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Rect_Instance
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];
	}VTXRECT_INSTANCE;

	typedef struct ENGINE_DLL tagVertex_Point_Instance
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	VertexElements[iNumElements];
	}VTXPOINT_INSTANCE;

	typedef struct tagNode_Description
	{
		unsigned int mNumChildren;
		unsigned int mNumMeshes;
		unsigned int* mMeshes;
		char         mName[1024];
		XMFLOAT4X4	mTransformation;


	}NODE_DESC;

	typedef struct tagWeights
	{
		unsigned int mVertexId;

		float mWeight;
	}VERTEX_WEIGHTS;

	typedef struct tagFaces
	{
		//! Number of indices defining this face.
		//! The maximum value for this member is #AI_MAX_FACE_INDICES.
		unsigned int mNumIndices;

		//! Pointer to the indices array. Size of the array is given in numIndices.
		unsigned int* mIndices;

	}FACE_DESC;

	typedef struct tagBoneDescription
	{
		XMFLOAT4X4 mOffsetMatrix;
		char	mName[1024];
		unsigned int mNumWeights;
		VERTEX_WEIGHTS* mWeights;

	}BONE_DESC;

	typedef struct tagMeshDescription
	{
		unsigned int mMaterialIndex;
		unsigned int mNumVertices;
		unsigned int mNumBones;
		unsigned int mNumFaces;
		XMFLOAT3* mVertices;
		XMFLOAT3* mNormals;
		XMFLOAT2* mTextureCoords;
		XMFLOAT3* mTangents;
		char mName[1024];
		BONE_DESC** mBones;
		FACE_DESC* mFaces;

	}MESH_DESC;

	typedef struct tagScalingKey
	{
		XMFLOAT3 mValue;
		double mTime;
	}SCALING_KEY;

	typedef struct tagPositionKey
	{
		XMFLOAT3 mValue;
		double mTime;
	}POSITION_KEY;

	typedef struct tagRotationKey
	{
		XMFLOAT4 mValue;
		double mTime;
	}ROTATION_KEY;

	typedef struct tagChannelDescription
	{
		char mNodeName[1024];
		unsigned int mNumScalingKeys;
		unsigned int mNumRotationKeys;
		unsigned int mNumPositionKeys;
		SCALING_KEY* mScalingKeys;
		ROTATION_KEY* mRotationKeys;
		POSITION_KEY* mPositionKeys;

	}CHANNEL_DESC;

	typedef struct tagAnimDescription
	{
		char mName[1024];
		double mDuration;
		double mTicksPerSecond;
		unsigned int mNumChannels;
		CHANNEL_DESC** mChannels;

	}ANIM_DESC;
}


