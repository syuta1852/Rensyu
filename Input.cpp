#include "Input.h"


Input::Input()
{
	ZeroMemory(keyState, sizeof(keyState));
}


Input::~Input()
{
	//�L�[�{�[�h�̃A�N�Z�X�������
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}

	//DirectInput���
	SAFE_RELEASE(pKeyDevice);
	SAFE_RELEASE(pDinput);
}


HRESULT Input::Init(HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return E_FAIL;
	}

	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(pDinput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	if (FAILED(pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������x���̐ݒ�
	if (FAILED(pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	//--�}�E�X�̏���--
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(pDinput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�o�C�X���}�E�X�ɐݒ�
	if (FAILED(pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������x���̐ݒ�
	if (FAILED(pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	m_hWnd = hWnd;

	return S_OK;
}

HRESULT  Input::Update()
{
	//�O��̃R���g���[���[�̏����m��
	memcpy(&prevControllerState, &controllerState, sizeof(controllerState));

	//�R���g���[���[�̏�Ԃ��擾
	for (int i = 0; i < 4; i++)
	{
		DWORD result = XInputGetState(i, &controllerState[i]);

		//���ڑ�
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			ZeroMemory(&controllerState[i], sizeof(XINPUT_STATE));
		}
	}

	// �f�o�C�X�̃A�N�Z�X�����擾����
	HRESULT hr = pKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		memcpy(prevKeyState, keyState, sizeof(keyState));

		//�S�ẴL�[�̏�Ԃ��擾
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
	}

	// �f�o�C�X�̃A�N�Z�X�����擾����
	hr = pMouseDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));

		//�S�ẴL�[�̏�Ԃ��擾
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	return E_FAIL;
}


BOOL Input::IsKeyPush(DWORD keyCode)
{
	if (keyState[keyCode] & 0x80)
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}


//�C�ӂ̃L�[�������ꂽ�u�Ԃ��`�F�b�N
BOOL  Input::IsKeyTap(DWORD keyCode)
{
	if (!(prevKeyState[keyCode] & 0x80)
		&& keyState[keyCode] & 0x80)
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�

}

//�C�ӂ̃L�[�������ꂽ�u�Ԃ��`�F�b�N
BOOL  Input::IsKeyRelease(DWORD keyCode)
{
	if ((prevKeyState[keyCode] & 0x80)
		&& !(keyState[keyCode] & 0x80))
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�

}

//�}�E�X�̔C�ӂ̃{�^����������Ă��邩�`�F�b�N
BOOL Input::IsMousePush(DWORD keyCode)
{
	if (mouseState.rgbButtons[keyCode] & 0x80)
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}


//�}�E�X�̔C�ӂ̃{�^���������ꂽ�u�Ԃ��`�F�b�N
BOOL  Input::IsMouseTap(DWORD keyCode)
{
	if (!(prevMouseState.rgbButtons[keyCode] & 0x80)
		&& mouseState.rgbButtons[keyCode] & 0x80)
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�

}

//�}�E�X�̔C�ӂ̃{�^�����͂Ȃ��ꂽ�u�Ԃ��`�F�b�N
BOOL  Input::IsMouseRelease(DWORD keyCode)
{
	if ((prevMouseState.rgbButtons[keyCode] & 0x80)
		&& !(mouseState.rgbButtons[keyCode] & 0x80))
	{
		return TRUE; //�����Ă���
	}
	return FALSE;      //�����ĂȂ�
}

//�}�E�X�J�[�\���𒆉��ɌŒ肷��֐�
void Input::SetCursorCenter()
{
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	SetCursorPos((rect.right - rect.left) / 2 + rect.left, (rect.bottom - rect.top) / 2 + rect.top);
}



BOOL Input::IsPadButtonPush(DWORD buttonCode, int ID)
{
	if (controllerState[ID].Gamepad.wButtons & buttonCode)
	{
		return TRUE; //�����Ă�
	}
	return FALSE;   //�����ĂȂ�
}

BOOL  Input::IsPadButtonTap(DWORD buttonCode, int ID)
{
	if (!(prevControllerState[ID].Gamepad.wButtons & buttonCode) &&
		controllerState[ID].Gamepad.wButtons & buttonCode)
	{
		return TRUE; //�����Ă�
	}
	return FALSE;   //�����ĂȂ�
}

BOOL  Input::IsPadButtonRelease(DWORD buttonCode, int ID)
{
	if ((prevControllerState[ID].Gamepad.wButtons & buttonCode) &&
		!(controllerState[ID].Gamepad.wButtons & buttonCode))
	{
		return TRUE; //�����Ă�
	}
	return FALSE;   //�����ĂȂ�
}

float Input::GetPadLStickX(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbLX;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32767;
}

float Input::GetPadLStickY(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbLY;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32767;
}

float Input::GetPadRStickX(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbRX;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		value > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32767;
}

float Input::GetPadRStickY(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbRY;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		value > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return 0.0f;
	}

	return value / 32767;
}

float Input::GetPadLTrigger(int ID)
{
	float value = controllerState[ID].Gamepad.bLeftTrigger;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;
	}

	return value / 255;
}

float Input::GetPadRTrigger(int ID)
{
	float value = controllerState[ID].Gamepad.bRightTrigger;

	//�f�b�h�]�[��
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;
	}

	return value / 255;
}

void Input::Vibration(float leftSpeed, float rightSpeed, int ID)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftSpeed * 65535; // �����[�^�[
	vibration.wRightMotorSpeed = rightSpeed * 65535; // �E���[�^�[
	XInputSetState(ID, &vibration);
}