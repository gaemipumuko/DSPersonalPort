#include "..\Public\GameObject.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_pGameInstance(rhs.m_pGameInstance)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (nullptr != pArg)
	{
		GAMEOBJECT_DESC		GameObjectDesc = *((GAMEOBJECT_DESC*)pArg);

		if (false == GameObjectDesc.strTransformTag.empty())
			m_strTransformTag = GameObjectDesc.strTransformTag;	

		m_pTransformCom->Initialize(&GameObjectDesc.TransformDesc);
	}

	m_Components.emplace(m_strTransformTag, m_pTransformCom);

	/* �׷��� ���۷��� ī��Ʈ�� ������Ű����. */
	Safe_AddRef(m_pTransformCom);

	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

/* ������ �����ؼ� ������� �纻��ü�� �ڽ��� ��������� �θ��� m_Components�� �߰��ϳ�. */
HRESULT CGameObject::Add_Components(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComTag, _Inout_ CComponent** ppComponent, void* pArg)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* �θ� ���� �±׸� ���� ������Ʈ�� �̹� �߰��Ǿ��־���?! */
	if (nullptr != Find_Component(strComTag))
		return E_FAIL;		
	
	/* ����������Ʈ�� �����ؿ�. */
	CComponent*		pComponent = m_pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	/* �����ؿ³��� �ڽ��� ��������� �������ֳ�. */
	*ppComponent = pComponent;

	/* �θ��� �� �����̳ʿ��� �߰��ϳ�. */	
	m_Components.emplace(strComTag, pComponent);

	/* �׷��� ���۷��� ī��Ʈ�� ������Ű����. */
	Safe_AddRef(pComponent);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const wstring & strComTag)
{
	auto	iter = m_Components.find(strComTag);

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;	
}

void CGameObject::Free()
{
	Safe_Release(m_pTransformCom);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
