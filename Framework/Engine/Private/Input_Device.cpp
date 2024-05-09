#include "Input_Device.h"

Engine::CInput_Device::CInput_Device(void)
{

}

_bool CInput_Device::Key_Press(int _iKey)
{

	if ((m_eKeyState[_iKey] == KEYSTATE::NONE) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::DOWN;
		return false;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::NONE) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::NONE;
		return false;
	}

	if ((m_eKeyState[_iKey] == KEYSTATE::DOWN) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::PRESS;
		return true;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::DOWN) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::RELEASE;
		return false;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::PRESS) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::RELEASE;
		return false;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::PRESS && (GetAsyncKeyState(_iKey) & 0x8001))
	{
		m_eKeyState[_iKey] = KEYSTATE::PRESS;
		return true;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::RELEASE && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::NONE;
		return false;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::RELEASE && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::DOWN;
		return false;
	}

}


_bool CInput_Device::Key_Down(int _iKey)
{
	if ((m_eKeyState[_iKey] == KEYSTATE::NONE) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::DOWN;
		return true;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::NONE) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::NONE;
		return false;
	}

	if ((m_eKeyState[_iKey] == KEYSTATE::DOWN) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::PRESS;
		return false;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::DOWN) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::RELEASE;
		return false;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::PRESS) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::RELEASE;
		return false;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::PRESS && (GetAsyncKeyState(_iKey) & 0x8001))
	{
		m_eKeyState[_iKey] = KEYSTATE::PRESS;
		return false;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::RELEASE && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::NONE;
		return false;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::RELEASE && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::DOWN;
		return true;
	}
}

_bool CInput_Device::Key_Release(int _iKey)
{
	if ((m_eKeyState[_iKey] == KEYSTATE::NONE) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::DOWN;
		return false;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::NONE) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::NONE;
		return false;
	}

	if ((m_eKeyState[_iKey] == KEYSTATE::DOWN) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::PRESS;
		return false;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::DOWN) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::RELEASE;
		return true;
	}
	if ((m_eKeyState[_iKey] == KEYSTATE::PRESS) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::RELEASE;
		return true;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::PRESS && (GetAsyncKeyState(_iKey) & 0x8001))
	{
		m_eKeyState[_iKey] = KEYSTATE::PRESS;
		return false;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::RELEASE && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::NONE;
		return false;
	}

	if (m_eKeyState[_iKey] == KEYSTATE::RELEASE && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_eKeyState[_iKey] = KEYSTATE::DOWN;
		return false;
	}
}

_bool CInput_Device::Get_Key_Down(_int _iKey)
{
	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = true;
			return true;
	}
	else if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = false;
	}
}

_bool CInput_Device::Get_Key_Press(_int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
	{
		return true;
	}
	return false;
}

_bool CInput_Device::Get_Key_Up(_int _iKey)
{
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = false;
		return true;
	}
	else if(!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = true;
	}
	//for (_int i = 0; i < 256; i++)
	//{
	//	if(!m_bKeyState[i] && (GetAsyncKeyState(_iKey) & 0x8000))
	//		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
	//}

	return false;
}

HRESULT Engine::CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{
	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		nullptr)))
		return E_FAIL;

	// 키보드 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInput_Device::Update_InputDev(void)
{
	/* 매 프레임마다 키보드와 마우스의 상태를 저장해준다. */

	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

CInput_Device * CInput_Device::Create(HINSTANCE hInst, HWND hWnd)
{
	CInput_Device*		pInstance = new CInput_Device();

	if (FAILED(pInstance->Initialize(hInst, hWnd)))
	{
		MSG_BOX("Failed to Created : CInput_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CInput_Device::Free(void)
{

	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

