#include "stdafx.h"
#include "..\Public\UI_Monster_HP_Base.h"

#include "GameInstance.h"

CUI_Monster_Hp_Base::CUI_Monster_Hp_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI_Monster_Hp_Base::CUI_Monster_Hp_Base(const CUI_Monster_Hp_Base & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CUI_Monster_Hp_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Monster_Hp_Base::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* 백그라운드 객체가 사용하고자하는 컴포너늩드릉ㄹ 궃가한다. */
	if (FAILED(Add_Component()))
		return E_FAIL;


	//_float2* fPos = (_float2*)pArg;
	//m_fX = fPos->x;
	//m_fY = fPos->y;

	m_fSizeX = 1.f;
	m_fSizeY = .07f;

	_vector* vPos = (_vector*)pArg;

	m_pHp_Bar = m_pHp_Bar->CUI_Monster_Hp_Bar::Create(m_pDevice, m_pContext, vPos);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//왼쪽 위 기준으로 하고싶으면 중점위치에서 사이즈 반만큼 더해서, 윈도우 절반만큼 빼는
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *vPos);


	return S_OK;
}

void CUI_Monster_Hp_Base::Tick(_float fTimeDelta)
{


}

void CUI_Monster_Hp_Base::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
	m_pHp_Bar->LateTick(fTimeDelta);
}

HRESULT CUI_Monster_Hp_Base::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

void CUI_Monster_Hp_Base::Update(_vector vPos, _float fHp, _uint iDamage)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Go_Up(0.53f);
	m_pTransformCom->Turn_Away(XMLoadFloat4(&m_pGameInstance->Get_CamPosition()));
	m_pHp_Bar->Update(m_pTransformCom->Get_State(CTransform::STATE_POSITION),fHp, 0);

}

HRESULT CUI_Monster_Hp_Base::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_Monster_Hp_Base"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Hp_Base::Bind_ShaderResources()
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

CUI_Monster_Hp_Base * CUI_Monster_Hp_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	/* 원형객체를 생성한다. */
	CUI_Monster_Hp_Base* pInstance = new CUI_Monster_Hp_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CUI_Monster_Hp_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Hp_Base::Clone(void* pArg)
{	
	/* 실사용객체를 생성한다. 복사생성자호출. */
	CUI_Monster_Hp_Base* pInstance = new CUI_Monster_Hp_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Monster_Hp_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Hp_Base::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pHp_Bar);
}
