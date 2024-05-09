#include "stdafx.h"
#include "MileStone_Blurr.h"

#include "GameInstance.h"

CMileStone_Blurr::CMileStone_Blurr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject(pDevice, pContext)
{

}

CMileStone_Blurr::CMileStone_Blurr(const CMileStone_Blurr& rhs)
    :CGameObject(rhs)
{
}

HRESULT CMileStone_Blurr::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMileStone_Blurr::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(&GAMEOBJECT_DESC(10.f, 90.0f))))
        return E_FAIL;

    if (FAILED(Add_Component()))
        return E_FAIL;

    
    if (pArg != nullptr)
        m_iAreaIndex = *(_uint*)pArg;

    m_fSizeX = g_iWinSizeX;
    m_fSizeY = 240.f;

    m_fX = g_iWinSizeX >> 1;
    m_fY = g_iWinSizeY >> 1;

    m_pTransformCom->Set_Scaled(_float3(m_fSizeX * 0.7f, m_fSizeY * 0.7f, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.0f, 300.f));


    return S_OK;
}

void CMileStone_Blurr::Tick(_float fTimedelta)
{
    if (m_bInitialized == true && m_bIsVisible == false)
    {
        Fade_In(fTimedelta,2.0f);
    }
    if (m_bIsVisible == true)
    {
        m_fVisible += fTimedelta;
    }
    if (m_bIsVisible == true && m_fVisible >= 2.f)
    {
        Fade_Out(fTimedelta,2.0f);
    }
}

void CMileStone_Blurr::LateTick(_float fTimedelta)
{
    m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CMileStone_Blurr::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(3)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CMileStone_Blurr::Add_Component()
{

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
    if (FAILED(__super::Add_Components(m_pGameInstance->Get_NextLevelIndex(), TEXT("Prototype_Component_MileStone_Blurr"),
        TEXT("Com_Texture_First"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMileStone_Blurr::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_ProjMatrix)))
        return E_FAIL;

    if(FAILED(m_pShaderCom->Bind_RawValue("g_fDeltaTime",&m_fFade_Time,sizeof(m_fFade_Time))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iAreaIndex)))
        return E_FAIL;

    return S_OK;
}

void CMileStone_Blurr::Fade_In(_float fTimeDelta, _float fFadeSec)
{
    if (m_fFade_Time < fFadeSec)
    {
        m_fFade_Time += fTimeDelta;
    }
    else
    {
        m_fFade_Time = fFadeSec;
        m_bIsVisible = true;
    }
}

void CMileStone_Blurr::Fade_Out(_float fTimeDelta, _float fFadeSec)
{
    m_bInitialized = false;
    if (m_fFade_Time >= 0)
        m_fFade_Time -= fTimeDelta;
    else
    {
        m_fFade_Time = 0.f;
        m_fVisible = 0.f;
        m_bIsVisible = false;
    }
}

CMileStone_Blurr* CMileStone_Blurr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMileStone_Blurr* pInstance = new CMileStone_Blurr(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMileStone_Blurr");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CMileStone_Blurr* CMileStone_Blurr::Clone(void* pArg)
{
    CMileStone_Blurr* pInstance = new CMileStone_Blurr(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CMileStone_Blurr");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMileStone_Blurr::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);

}
