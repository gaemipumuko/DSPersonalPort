#include "stdafx.h"
#include "Bonfire.h"
#include "Sound_Manager.h"

#include "GameInstance.h"

CBonfire::CBonfire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBonfire::CBonfire(const CBonfire& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBonfire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBonfire::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_vector*)pArg);
	}

	return S_OK;
}

void CBonfire::Tick(_float fTimeDelta)
{
	CTransform* pPlayer_Transform = (CTransform*)m_pGameInstance->Get_Component(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pPlayer_Transform->Get_State(CTransform::STATE_POSITION);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vToPlayer = vPlayerPos - vPosition;

	if (XMVector3Length(vToPlayer).m128_f32[0] <= 4.5f)
	{
		if (m_bIsApproched)
		{
			wstring strOK = TEXT(": 사용하기");
			if (m_pInteraction_Box == nullptr)
			{
				m_pInteraction_Box = CInteraction::Create(m_pDevice, m_pContext, &strOK);
			}
			m_bIsApproched = false;
		}

		if (m_pGameInstance->Key_Down('E'))
		{
			CSound_Manager::Get_Instance()->PlaySound(L"ITEMGET.wav",CSound_Manager::SOUND_BACKGROUND_EFFECT1,0.5f);


			m_bChangeMap = true;

		}

	}
	else
	{
		m_bIsApproched = true;
	}


	if (m_bChangeMap == true)
	{
		m_fChangeDelay += fTimeDelta;

		if (m_fChangeDelay >= 1.f)
		{
			if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_FIRELINK)
			{
				m_pGameInstance->Ready_Next_Level(LEVEL_GAMEPLAY);
			}
			else if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_GAMEPLAY)
			{
				m_pGameInstance->Ready_Next_Level(LEVEL_GUNDYR);
			}
			else if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_GUNDYR)
			{
				m_pGameInstance->Ready_Next_Level(LEVEL_FIRELINK, true);
			}
		}
	}
	//if (m_pGameInstance->Get_DIKeyState(DIK_LEFT) & 0x80)
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 5.f);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT) & 0x80)
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * 5.f);
	//}


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vector{ 200.f,100.f,200.f,1.f });

}

void CBonfire::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	if (m_bIsApproched == false)
	{
		m_pInteraction_Box->LateTick(fTimeDelta);
	}
}

HRESULT CBonfire::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CBonfire::Add_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bonfire"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBonfire::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CBonfire* CBonfire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CBonfire* pInstance = new CBonfire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBonfire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBonfire::Clone(void* pArg)
{
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CBonfire* pInstance = new CBonfire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBonfire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBonfire::Free()
{
	__super::Free();

	Safe_Release(m_pInteraction_Box);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
