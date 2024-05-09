#include "stdafx.h"
#include "..\Public\Inventory_Base.h"

#include "GameInstance.h"
#include "Player.h"

CInventory_Base::CInventory_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CInventory_Base::CInventory_Base(const CInventory_Base & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CInventory_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory_Base::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
		return E_FAIL;


	/* ��׶��� ��ü�� ����ϰ����ϴ� �����ʈp�帪�� �����Ѵ�. */
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 1280.f;
	m_fSizeY = 720.f;

	m_fX = 0;
	m_fY = 0;

	_float4 vSquare_Dimentions;

	vSquare_Dimentions.x = 122.f;
	vSquare_Dimentions.y = 213.f;
	vSquare_Dimentions.z = 56.f;
	vSquare_Dimentions.w = 64.f;

	m_pSelect_Square = CSelect_Square::Create(m_pDevice, m_pContext, &vSquare_Dimentions);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//���� �� �������� �ϰ������ ������ġ���� ������ �ݸ�ŭ ���ؼ�, ������ ���ݸ�ŭ ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX + m_fSizeX/2 - g_iWinSizeX*0.5 , -m_fY - m_fSizeY/2 + g_iWinSizeY*0.5, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());	
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));
	
	return S_OK;
}

void CInventory_Base::Tick(_float fTimeDelta)
{
	_float fResX = 1280.f / 1916.f;
	_float fResY = 720.f / 1080.f;

	m_pPlayerParam = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(m_pGameInstance->Get_NextLevelIndex(), TEXT("Layer_Player"), TEXT("Player")))->Get_PlayerParam();

	if (m_pGameInstance->Key_Down('Q'))
	{
		m_bDestroy = true;
		m_pPlayerParam = nullptr;
	}

	if (m_pGameInstance->Key_Down(VK_UP))
	{
		if (m_iMenuIndex >= 5)
		{
			m_iMenuIndex -= 5;
		}
	}
	if (m_pGameInstance->Key_Down(VK_DOWN))
	{
		if (m_iMenuIndex <= 24)
		{
			m_iMenuIndex += 5;
		}
	}
	if (m_pGameInstance->Key_Down(VK_LEFT))
	{
		if (m_iMenuIndex > 0 && m_iMenuIndex != 5 && m_iMenuIndex != 10 && m_iMenuIndex != 15 && m_iMenuIndex != 20 && m_iMenuIndex != 25)
		{
			m_iMenuIndex--;
		}
	}
	if (m_pGameInstance->Key_Down(VK_RIGHT))
	{
		if (m_iMenuIndex < 29 && m_iMenuIndex != 4 && m_iMenuIndex != 9 && m_iMenuIndex != 14 && m_iMenuIndex != 19 && m_iMenuIndex != 24 && m_iMenuIndex != 29)
		{
			m_iMenuIndex++;
		}
	}

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	switch (m_iMenuIndex)
	{

	case 0:
		m_pSelect_Square->Set_Position(m_fX + 122.f* fResX, m_fY + 213.f* fResY);
		break;
	case 1:
		m_pSelect_Square->Set_Position(m_fX + 222.f * fResX , m_fY + 213.f* fResY);
		break;
	case 2:
		m_pSelect_Square->Set_Position(m_fX + 322.f * fResX, m_fY + 213.f * fResY);
		break;
	case 3:
		m_pSelect_Square->Set_Position(m_fX + 450.f * fResX, m_fY + 213.f * fResY);
		break;
	case 4:
		m_pSelect_Square->Set_Position(m_fX + 549.f * fResX, m_fY + 213.f * fResY);
		break;
	case 5:
		m_pSelect_Square->Set_Position(m_fX + 121.f * fResX, m_fY + 323.f * fResY);
		break;
	case 6:
		m_pSelect_Square->Set_Position(m_fX + 220.f * fResX, m_fY + 323.f * fResY);
		break;
	case 7:
		m_pSelect_Square->Set_Position(m_fX + 320.f * fResX, m_fY + 323.f * fResY);
		break;
	case 8:
		m_pSelect_Square->Set_Position(m_fX + 449.f * fResX, m_fY + 323.f * fResY);
		break;
	case 9:
		m_pSelect_Square->Set_Position(m_fX + 549.f * fResX, m_fY + 323.f * fResY);
		break;
	case 10:
		m_pSelect_Square->Set_Position(m_fX + 121.f * fResX, m_fY + 450.f * fResY);
		break;
	case 11:
		m_pSelect_Square->Set_Position(m_fX + 221.f * fResX, m_fY + 450.f * fResY);
		break;
	case 12:
		m_pSelect_Square->Set_Position(m_fX + 321.f * fResX, m_fY + 450.f * fResY);
		break;
	case 13:
		m_pSelect_Square->Set_Position(m_fX + 420.f * fResX, m_fY + 450.f * fResY);
		break;
	case 14:
		m_pSelect_Square->Set_Position(m_fX + 557.f * fResX, m_fY + 513.f * fResY);
		break;
	case 15:
		m_pSelect_Square->Set_Position(m_fX + 121.f * fResX, m_fY + 576.f * fResY);
		break;
	case 16:
		m_pSelect_Square->Set_Position(m_fX + 221.f * fResX, m_fY + 576.f * fResY);
		break;
	case 17:
		m_pSelect_Square->Set_Position(m_fX + 321.f * fResX, m_fY + 576.f * fResY);
		break;
	case 18:
		m_pSelect_Square->Set_Position(m_fX + 420.f * fResX, m_fY + 576.f * fResY);
		break;
	case 19:
		m_pSelect_Square->Set_Position(m_fX + 557.f * fResX, m_fY + 513.f * fResY);
		break;
	case 20:
		m_pSelect_Square->Set_Position(m_fX + 121.f * fResX, m_fY + 702.f * fResY);
		break;
	case 21:
		m_pSelect_Square->Set_Position(m_fX + 221.f * fResX, m_fY + 702.f * fResY);
		break;
	case 22:
		m_pSelect_Square->Set_Position(m_fX + 321.f * fResX, m_fY + 702.f * fResY);
		break;
	case 23:
		m_pSelect_Square->Set_Position(m_fX + 420.f * fResX, m_fY + 702.f * fResY);
		break;
	case 24:
		m_pSelect_Square->Set_Position(m_fX + 520.f * fResX, m_fY + 702.f * fResY);
		break;
	case 25:
		m_pSelect_Square->Set_Position(m_fX + 121.f * fResX, m_fY + 812.f * fResY);
		break;
	case 26:
		m_pSelect_Square->Set_Position(m_fX + 221.f * fResX, m_fY + 812.f * fResY);
		break;
	case 27:
		m_pSelect_Square->Set_Position(m_fX + 321.f * fResX, m_fY + 812.f * fResY);
		break;
	case 28:
		m_pSelect_Square->Set_Position(m_fX + 420.f * fResX, m_fY + 812.f * fResY);
		break;
	case 29:
		m_pSelect_Square->Set_Position(m_fX + 520.f * fResX, m_fY + 812.f * fResY);
		break;
	default:
		m_iMenuIndex = 0;
		break;

	}

	m_pSelect_Square->Tick(fTimeDelta);

}

void CInventory_Base::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CInventory_Base::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#pragma region �ɷ�ġ ���

	if(m_pPlayerParam != nullptr)
	{ 
	_tchar szCurrentWeight[MAX_PATH], szMaxWeight[MAX_PATH], szWeightPercent[MAX_PATH], szLevel[MAX_PATH], szVigor[MAX_PATH], szAttunement[MAX_PATH], szEndurance[MAX_PATH];
	_tchar szVitality[MAX_PATH], szStrength[MAX_PATH], szDexterity[MAX_PATH], szIntelligence[MAX_PATH], szFaith[MAX_PATH], szLuck[MAX_PATH], szMemorySlot[MAX_PATH];
	_tchar szCurrentHp[MAX_PATH], szMaxHp[MAX_PATH], szCurrentFp[MAX_PATH], szMaxFp[MAX_PATH], szStamina[MAX_PATH], szRigidity[MAX_PATH], szFind[MAX_PATH];

	wsprintf(szCurrentWeight, TEXT("%d /"), m_pPlayerParam->fCurrentWeight);
	wsprintf(szMaxWeight, TEXT("%d"), m_pPlayerParam->fMaxWeight);
	wsprintf(szWeightPercent, TEXT("%d%"), (m_pPlayerParam->fCurrentWeight / m_pPlayerParam->fMaxWeight)/100);

	wsprintf(szLevel, TEXT("%d"), m_pPlayerParam->iLevel);

	wsprintf(szVigor, TEXT("%d"), m_pPlayerParam->iVigor);
	wsprintf(szAttunement, TEXT("%d"), m_pPlayerParam->iAttunement);
	wsprintf(szEndurance, TEXT("%d"), m_pPlayerParam->iEndurance);
	wsprintf(szVitality, TEXT("%d"), m_pPlayerParam->iVitality);
	wsprintf(szStrength, TEXT("%d"), m_pPlayerParam->iStrength);
	wsprintf(szDexterity, TEXT("%d"), m_pPlayerParam->iDexterity);
	wsprintf(szIntelligence, TEXT("%d"), m_pPlayerParam->iIntelligence);
	wsprintf(szFaith, TEXT("%d"), m_pPlayerParam->iFaith);
	wsprintf(szLuck, TEXT("%d"), m_pPlayerParam->iLuck);

	wsprintf(szCurrentHp, TEXT("%d /"), m_pPlayerParam->fCurrentHp);
	wsprintf(szMaxHp, TEXT("%d"), m_pPlayerParam->fMaxHp);
	wsprintf(szCurrentFp, TEXT("%d /"), m_pPlayerParam->fCurrentFp);
	wsprintf(szMaxFp, TEXT("%d"), m_pPlayerParam->fMaxFp);
	wsprintf(szStamina, TEXT("%d"), m_pPlayerParam->fMaxStamina);

	wsprintf(szRigidity, TEXT("%d"), m_pPlayerParam->fRigidity);
	wsprintf(szFind, TEXT("%d"), m_pPlayerParam->fFind);
	wsprintf(szMemorySlot, TEXT("%d"), m_pPlayerParam->iMemorySlots);


	_float fLength = wcslen(szCurrentWeight);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szCurrentWeight), _float2(1160.f - fLength * 10, 42.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szMaxWeight), _float2(1200.f - fLength * 10, 42.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szWeightPercent), _float2(1200.f - fLength * 10, 63.f), 0.4f, Colors::White);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szLevel), _float2(1217.f - fLength * 10, 140.f), 0.4f, Colors::White);
	
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szVigor), _float2(1215.f - fLength * 10, 188.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szAttunement), _float2(1215.f - fLength * 10, 208.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szEndurance), _float2(1215.f - fLength * 10, 228.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szVitality), _float2(1215.f - fLength * 10, 250.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szStrength), _float2(1215.f - fLength * 10, 268.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szDexterity), _float2(1215.f - fLength * 10, 288.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szIntelligence), _float2(1218.f - fLength * 10, 308.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szFaith), _float2(1218.f - fLength * 10, 328.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szLuck), _float2(1218.f - fLength * 10, 348.f), 0.4f, Colors::White);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szCurrentHp), _float2(1160.f - fLength * 10, 388.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szMaxHp), _float2(1208.f - fLength * 10, 388.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szCurrentFp), _float2(1160.f - fLength * 10, 410.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szMaxFp), _float2(1208.f - fLength * 10, 410.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szStamina), _float2(1208.f - fLength * 10, 432.f), 0.4f, Colors::White);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szCurrentWeight), _float2(1160.f - fLength * 10, 483.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szMaxWeight), _float2(1208.f - fLength * 10, 483.f), 0.4f, Colors::White);

	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szRigidity), _float2(1208.f - fLength * 10, 503.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szFind), _float2(1208.f - fLength * 10, 523.f), 0.4f, Colors::White);
	m_pGameInstance->Render_Text(TEXT("Font_Default"), wstring(szMemorySlot), _float2(1208.f - fLength * 10, 543.f), 0.4f, Colors::White);

	}



#pragma endregion

	m_pSelect_Square->Render();
	return S_OK;
}

HRESULT CInventory_Base::Add_Component()
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
	if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_Texture_Inventory_Base"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory_Base::Bind_ShaderResources()
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

CInventory_Base * CInventory_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* ������ü�� �����Ѵ�. */
	CInventory_Base* pInstance = new CInventory_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInventory_Base::Clone(void* pArg)
{	
	/* �ǻ�밴ü�� �����Ѵ�. ���������ȣ��. */
	CInventory_Base* pInstance = new CInventory_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInventory_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory_Base::Free()
{
	__super::Free();

	Safe_Release(m_pSelect_Square);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
