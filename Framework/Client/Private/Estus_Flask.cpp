#include "stdafx.h"
#include "..\Public\Estus_Flask.h"
#include "Sound_Manager.h"

#include "GameInstance.h"
#include "Player.h"

CEstus_Flask::CEstus_Flask(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CEstus_Flask::CEstus_Flask(const CEstus_Flask & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CEstus_Flask::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEstus_Flask::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 85.f;
	m_fSizeY = 110.f;

	m_fX = 145;
	m_fY = 555;


	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CEstus_Flask::Tick(_float fTimeDelta)
{

	CPlayer* m_pPlayer = (CPlayer*)m_pGameInstance->Get_Object(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Player"));

	if (m_pGameInstance->Get_Key_Up('R'))
	{

		if (m_iEstus_Charge > 0)
		{
			m_pGameInstance->Add_Layers(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Particle_Rect"));
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_ITEM);
			CSound_Manager::Get_Instance()->PlaySound(L"Estus.wav",CSound_Manager::SOUND_ITEM,1.f);
			m_pPlayer->Restore_Hp(150.f);
			m_iEstus_Charge--;
		}
	}

	
	switch (m_iEstus_Charge)
	{
	case 0:
		m_iImage_Index = 0;
		break;
	case 1:
		m_iImage_Index = 1;
		break;
	case 2:
		m_iImage_Index = 1;
		break;
	case 3:
		m_iImage_Index = 2;
		break;
	case 4:
		m_iImage_Index = 2;
		break;
	case 5:
		m_iImage_Index = 3;
		break;
	default:
		break;
	}
}

void CEstus_Flask::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CEstus_Flask::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#pragma region 갯수 출력

	wsprintf(m_szCharge, TEXT("%d"), m_iEstus_Charge);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(m_szCharge), _float2(203.f, 625.f), 0.5f, Colors::White);

#pragma endregion

//#pragma region 이름 출력
//
//	wsprintf(m_szName, TEXT("에스트 병"));
//
//	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(m_szName), _float2(150.f, 655.f), 0.5f, Colors::White);
//
//#pragma endregion


	return S_OK;
}

HRESULT CEstus_Flask::Add_Component()
{
	/* Clone은 새로운 복사본객체를 만든다. */
	/* 다만 렌더러의 경우에는 원본의 주소를 리턴하낟. Clone함수안에서 AddRef가 호출되도록. */
	/* 내 멤버변수에도 컴포넌트를 저장하고, 부모에게도 태그(키)와 함께 저장한다. */

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;	

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_Estus_Flask"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEstus_Flask::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iImage_Index)))
		return E_FAIL;

	return S_OK;
}

CEstus_Flask * CEstus_Flask::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 원형객체를 생성한다. */
	CEstus_Flask* pInstance = new CEstus_Flask(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEstus_Flask");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEstus_Flask::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CEstus_Flask* pInstance = new CEstus_Flask(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEstus_Flask");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEstus_Flask::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
