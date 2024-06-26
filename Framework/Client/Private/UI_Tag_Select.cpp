#include "stdafx.h"
#include "..\Public\UI_Tag_Select.h"

#include "GameInstance.h"

CSelect_Tag::CSelect_Tag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CSelect_Tag::CSelect_Tag(const CSelect_Tag & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CSelect_Tag::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSelect_Tag::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	_float4 fSquare_Dimentions = *(_float4*)pArg;

	m_fSizeX = fSquare_Dimentions.z;
	m_fSizeY = fSquare_Dimentions.w;

	m_fX = fSquare_Dimentions.x;
	m_fY = fSquare_Dimentions.y;


	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CSelect_Tag::Tick(_float fTimeDelta)
{
	
	if ((m_pGameInstance->Get_DIKeyState(DIK_UP) & 0x80))
	{
		if (m_fIndex == 1)
		{
			Set_Position(168.f, 172.f);
			m_fIndex = 0;
		}
		else
		{
			m_fIndex = 0;
		}
	}

	if(m_pGameInstance->Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		if (m_fIndex == 0)
		{
			Set_Position(168.f, 208.f);
			m_fIndex = 1;
		}
		else
		{
			m_fIndex = 1;
		}
	}

	if (m_bFade == true)
	{
		m_fFade_Time -= fTimeDelta;
		if (m_fFade_Time <= 1.f)
		{
			m_fFade_Time = 1.f;
			m_bFade = false;
		}
	}
	else if (m_bFade == false)
	{
		m_fFade_Time += fTimeDelta;
		if (m_fFade_Time >= 3.f)
		{
			m_fFade_Time = 3.f;
			m_bFade = true;
		}
	}

}

void CSelect_Tag::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CSelect_Tag::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CSelect_Tag::Set_Position(_float fx, _float fy)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fx + m_fSizeX / 2 - g_iWinSizeX * 0.5, -fy - m_fSizeY / 2 + g_iWinSizeY * 0.5, 0.f, 1.f));
}

HRESULT CSelect_Tag::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_UI_Select_Tag"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelect_Tag::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDeltaTime", &m_fFade_Time, sizeof(m_fFade_Time))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	return S_OK;
}

//CSelect_Tag * CSelect_Tag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//{
//	/* 원형객체를 생성한다. */
//	CSelect_Tag* pInstance = new CSelect_Tag(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created : CSelect_Tag");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CSelect_Tag* CSelect_Tag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	/* 원형객체를 생성한다. */
	CSelect_Tag* pInstance = new CSelect_Tag(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSelect_Tag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSelect_Tag::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CSelect_Tag* pInstance = new CSelect_Tag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSelect_Tag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSelect_Tag::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
