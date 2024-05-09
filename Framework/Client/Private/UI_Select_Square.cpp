#include "stdafx.h"
#include "..\Public\UI_Select_Square.h"

#include "GameInstance.h"
#include "Player.h"

CSelect_Square::CSelect_Square(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CSelect_Square::CSelect_Square(const CSelect_Square & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CSelect_Square::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSelect_Square::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* ��׶��� ��ü�� ����ϰ����ϴ� �����ʈp�帪�� �����Ѵ�. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	_float4 fSquare_Dimentions = *(_float4*)pArg;

	m_fSizeX = fSquare_Dimentions.z;
	m_fSizeY = fSquare_Dimentions.w;

	m_fX = fSquare_Dimentions.x;
	m_fY = fSquare_Dimentions.y;


	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//���� �� �������� �ϰ������ ������ġ���� ������ �ݸ�ŭ ���ؼ�, ������ ���ݸ�ŭ ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));

	m_fFade_Time = 3.f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CSelect_Square::Tick(_float fTimeDelta)
{
	
	if (m_bFade == true)
	{
		m_fFade_Time -= fTimeDelta*2;
		if (m_fFade_Time <= 1.f)
		{
			m_fFade_Time = 1.f;
			m_bFade = false;
		}
	}
	else if (m_bFade == false)
	{
		m_fFade_Time += fTimeDelta*2;
		if (m_fFade_Time >= 3.f)
		{
			m_fFade_Time = 3.f;
			m_bFade = true;
		}
	}

}

void CSelect_Square::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CSelect_Square::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CSelect_Square::Set_Position(_float fx, _float fy)
{
	//XMVECTOR fPosition = XMVectorSet(fx, fy, 0.f, 1.f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPosition);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fx + m_fSizeX / 2 - g_iWinSizeX * 0.5, -fy - m_fSizeY / 2 + g_iWinSizeY * 0.5, 0.f, 1.f));
}

HRESULT CSelect_Square::Add_Component()
{
	/* Clone�� ���ο� ���纻��ü�� �����. */
	/* �ٸ� �������� ��쿡�� ������ �ּҸ� �����ϳ�. Clone�Լ��ȿ��� AddRef�� ȣ��ǵ���. */
	/* �� ����������� ������Ʈ�� �����ϰ�, �θ𿡰Ե� �±�(Ű)�� �Բ� �����Ѵ�. */

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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_UI_Select_Square"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelect_Square::Bind_ShaderResources()
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

//CSelect_Square * CSelect_Square::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//{
//	/* ������ü�� �����Ѵ�. */
//	CSelect_Square* pInstance = new CSelect_Square(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created : CSelect_Square");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CSelect_Square* CSelect_Square::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	/* ������ü�� �����Ѵ�. */
	CSelect_Square* pInstance = new CSelect_Square(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSelect_Square");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSelect_Square::Clone(void* pArg)
{	
	/* �ǻ�밴ü�� �����Ѵ�. ���������ȣ��. */
	CSelect_Square* pInstance = new CSelect_Square(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSelect_Square");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSelect_Square::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
