#include "stdafx.h"
#include "..\Public\UI_NPC_Base.h"
#include "Sound_Manager.h"

#include "GameInstance.h"

CUI_NPC_Base::CUI_NPC_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI_NPC_Base::CUI_NPC_Base(const CUI_NPC_Base & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CUI_NPC_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_NPC_Base::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* ��׶��� ��ü�� ����ϰ����ϴ� �����ʈp�帪�� �����Ѵ�. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eNPCType = *(NPC_TYPE*)pArg;

	m_fSizeX = 415.f;
	m_fSizeY = 330.f;

	m_fX = 113.f;
	m_fY = 82.f;
	
	CUI_NPC_Icon::NPC_ICON_TYPE eNpcIcon = CUI_NPC_Icon::NPC_ICON_BONFIRE;

	switch (m_eNPCType)
	{
	case Client::CUI_NPC_Base::NPC_BONFIRE:
		m_strObjName = TEXT("���� ����� ������");
		eNpcIcon = CUI_NPC_Icon::NPC_ICON_BONFIRE;
		m_iMenuIndex = 3;
		break;
	case Client::CUI_NPC_Base::NPC_SHOP:
		m_strObjName = TEXT("�������� �ó�");
		eNpcIcon = CUI_NPC_Icon::NPC_ICON_SHOP;
		m_iMenuIndex = 3;
		break;
	case Client::CUI_NPC_Base::NPC_BLACKSMITH:
		m_strObjName = TEXT("�������� �ȵ巹��");
		eNpcIcon = CUI_NPC_Icon::NPC_ICON_BLACKSMITH;
		m_iMenuIndex = 5; 
		break;
	case Client::CUI_NPC_Base::NPC_FIREKEEPER:
		m_strObjName = TEXT("ȭ���");
		eNpcIcon = CUI_NPC_Icon::NPC_ICON_FIREKEEPER; 
		m_iMenuIndex = 3;
		break;
	case Client::CUI_NPC_Base::NPC_END:
		break;
	default:
		break;
	}

	m_pNPCIcon = CUI_NPC_Icon::Create(m_pDevice, m_pContext, &eNpcIcon);

	_float4 vSquare_Dimentions;

	vSquare_Dimentions.x = 168.f;
	vSquare_Dimentions.y = 172.f;
	vSquare_Dimentions.z = 306.f;
	vSquare_Dimentions.w = 25.f;

	m_pSelect_Tag = CSelect_Tag::Create(m_pDevice, m_pContext, &vSquare_Dimentions);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//���� �� �������� �ϰ������ ������ġ���� ������ �ݸ�ŭ ���ؼ�, ������ ���ݸ�ŭ ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CUI_NPC_Base::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(VK_DOWN) && m_iCurrentMenuIndex>0)
	{
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_BACKGROUND_EFFECT1);
		CSound_Manager::Get_Instance()->PlaySound(L"CURSOL_CLICK.wav", CSound_Manager::SOUND_BACKGROUND_EFFECT2, 1.f);

		m_iCurrentMenuIndex++;
	}
	if (m_pGameInstance->Key_Down(VK_UP) && m_iCurrentMenuIndex < m_iCurrentMenuIndex)
	{
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::SOUND_BACKGROUND_EFFECT2);
		CSound_Manager::Get_Instance()->PlaySound(L"CURSOL_CLICK.wav", CSound_Manager::SOUND_BACKGROUND_EFFECT1, 1.f);

		m_iCurrentMenuIndex--;
	}

	if (m_pGameInstance->Key_Down(VK_RETURN) && m_iCurrentMenuIndex == m_iCurrentMenuIndex - 1)
	{

	}
	m_pSelect_Tag->Tick(fTimeDelta);
}

void CUI_NPC_Base::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_NPC_Base::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pNPCIcon->Render();
	m_pSelect_Tag->Render();

	return S_OK;
}

HRESULT CUI_NPC_Base::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_UI_NPC_Base"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_NPC_Base::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	return S_OK;
}

CUI_NPC_Base * CUI_NPC_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	/* ������ü�� �����Ѵ�. */
	CUI_NPC_Base* pInstance = new CUI_NPC_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CUI_NPC_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_NPC_Base::Clone(void* pArg)
{	
	/* �ǻ�밴ü�� �����Ѵ�. ���������ȣ��. */
	CUI_NPC_Base* pInstance = new CUI_NPC_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_NPC_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_NPC_Base::Free()
{
	Safe_Release(m_pNPCIcon);
	Safe_Release(m_pSelect_Tag);
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
