#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CMileStone_Text final : public CGameObject
{

private:
	CMileStone_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMileStone_Text(const CMileStone_Text& rhs);
	virtual ~CMileStone_Text() = default;

private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimedelta) override;
	virtual void LateTick(_float fTimedelta) override;
	virtual HRESULT Render() override;

private:
	CShader*		m_pShaderCom = {nullptr};
	CTexture*		m_pTextureCom = {};
	CRenderer*		m_pRendererCom = { };
	CVIBuffer_Rect* m_pVIBufferCom = { };

private:
	_float					m_fIndex = { 0.f };
	_float					m_fSizeX, m_fSizeY;
	_float					m_fX, m_fY;
	_float					m_fFade_Time = {};
	_float					m_fVisible = {};

	_float4x4				m_ViewMatrix, m_ProjMatrix;

	_bool					m_bIsVisible = {false};
	_bool					m_bInitialized = { true };

	_uint					m_iAreaIndex = {};

private:
	HRESULT Add_Component();
	HRESULT Bind_ShaderResources();

	void	Fade_In(_float fTimeDelta, _float fFadeSec);
	void	Fade_Out(_float fTimeDelta, _float fFadeSec);

public:
	static CMileStone_Text* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CMileStone_Text* Clone(void* pArg) override;
	virtual void Free();

};

END
