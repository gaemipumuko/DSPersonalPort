#include "stdafx.h"
#include "..\Public\UI_Monster_Hp_Bar.h"
#include "Player.h"

#include "GameInstance.h"

CUI_Monster_Hp_Bar::CUI_Monster_Hp_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI_Monster_Hp_Bar::CUI_Monster_Hp_Bar(const CUI_Monster_Hp_Bar & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CUI_Monster_Hp_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Monster_Hp_Bar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* ��׶��� ��ü�� ����ϰ����ϴ� �����ʈp�帪�� �����Ѵ�. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 0.95f;
	m_fSizeY = 0.04f;
	//_float2* fPos = (_float2*)pArg;
	// 
	//m_fX = fPos->x;
	//m_fY = fPos->y;
	
	_vector* vPos = (_vector*)pArg;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//���� �� �������� �ϰ������� ������ġ���� ������ �ݸ�ŭ ���ؼ�, ������ ���ݸ�ŭ ����
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *vPos);

	return S_OK;
}

void CUI_Monster_Hp_Bar::Tick(_float fTimeDelta)
{

	/*m_pPlayerParam = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player")))->Get_PlayerParam();
	m_fSizeX = 176.f * (m_pPlayerParam->fCurrentHp) / (m_pPlayerParam->fMaxHp) ;
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX / 2 - g_iWinSizeX * 0.5, -m_fY - m_fSizeY / 2 + g_iWinSizeY * 0.5, 0.f, 1.f));*/
}

void CUI_Monster_Hp_Bar::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_Monster_Hp_Bar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

void CUI_Monster_Hp_Bar::Update(_vector vPos, _float fHp, _uint iDamage)
{
	m_pTransformCom->Turn_Away(XMLoadFloat4(&m_pGameInstance->Get_CamPosition()));
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX*fHp, m_fSizeY, 1.f));
	_float fCenterDist = (0.95f - (0.95f * fHp))/2;
	vPos.m128_f32[0] = vPos.m128_f32[0] - (fCenterDist);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

HRESULT CUI_Monster_Hp_Bar::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_Player_Hp_Bar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Hp_Bar::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	return S_OK;
}

CUI_Monster_Hp_Bar * CUI_Monster_Hp_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	/* ������ü�� �����Ѵ�. */
	CUI_Monster_Hp_Bar* pInstance = new CUI_Monster_Hp_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CUI_Monster_Hp_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Hp_Bar::Clone(void* pArg)
{	
	/* �ǻ�밴ü�� �����Ѵ�. ���������ȣ��. */
	CUI_Monster_Hp_Bar* pInstance = new CUI_Monster_Hp_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Monster_Hp_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Hp_Bar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	
}