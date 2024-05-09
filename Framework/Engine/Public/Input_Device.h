#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

BEGIN(Engine)

class CInput_Device : public CBase
{

	enum KEYSTATE
	{
		NONE,
		DOWN,
		PRESS,
		RELEASE
	};

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
	_bool Key_Press(int _iKey);
	_bool Key_Down(int _iKey);
	_bool Key_Release(int _iKey);

	_bool Get_Key_Down(_int _iKey);
	_bool Get_Key_Press(_int _iKey);
	_bool Get_Key_Up(_int _iKey);
public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void); /* 키보드와 마우스의 상태를 저장한다. */

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;
	

private:
	_int					m_eKeyState[256] = {};
	_bool					m_bKeyState[256] = {};
	_byte					m_byKeyState[256];			// 키보드에 있는 모든 키값을 저장하기 위한 변수
	_byte					m_byPrevKeyState[256];		// 이전 프레임의 키값을 저장하기 위한 변수
	DIMOUSESTATE			m_tMouseState;	

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};
END
#endif // InputDev_h__
