#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

BEGIN(Engine)

class CInput_Device : public CBase
{

private:
	CInput_Device(void);	
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID) const { 
		return m_byKeyState[byKeyID]; 
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) const { return m_tMouseState.rgbButtons[eMouse]; 	}
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	const
	{	
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}
	
public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void); /* Ű����� ���콺�� ���¸� �����Ѵ�. */

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;
	

private:
	_byte					m_byKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
	DIMOUSESTATE			m_tMouseState;	

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};
END
#endif // InputDev_h__
