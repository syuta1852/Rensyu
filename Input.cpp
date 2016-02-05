#include "Input.h"


Input::Input()
{
	ZeroMemory(keyState, sizeof(keyState));
}


Input::~Input()
{
	//キーボードのアクセス権を解放
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}

	//DirectInput解放
	SAFE_RELEASE(pKeyDevice);
	SAFE_RELEASE(pDinput);
}


HRESULT Input::Init(HWND hWnd)
{
	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return E_FAIL;
	}

	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(pDinput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL)))
	{
		return E_FAIL;
	}

	// デバイスをキーボードに設定
	if (FAILED(pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調レベルの設定
	if (FAILED(pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	//--マウスの準備--
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(pDinput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL)))
	{
		return E_FAIL;
	}

	// デバイスをマウスに設定
	if (FAILED(pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調レベルの設定
	if (FAILED(pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	m_hWnd = hWnd;

	return S_OK;
}

HRESULT  Input::Update()
{
	//前回のコントローラーの情報を確保
	memcpy(&prevControllerState, &controllerState, sizeof(controllerState));

	//コントローラーの状態を取得
	for (int i = 0; i < 4; i++)
	{
		DWORD result = XInputGetState(i, &controllerState[i]);

		//未接続
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			ZeroMemory(&controllerState[i], sizeof(XINPUT_STATE));
		}
	}

	// デバイスのアクセス権を取得する
	HRESULT hr = pKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		memcpy(prevKeyState, keyState, sizeof(keyState));

		//全てのキーの状態を取得
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
	}

	// デバイスのアクセス権を取得する
	hr = pMouseDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));

		//全てのキーの状態を取得
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	return E_FAIL;
}


BOOL Input::IsKeyPush(DWORD keyCode)
{
	if (keyState[keyCode] & 0x80)
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}


//任意のキーが押された瞬間かチェック
BOOL  Input::IsKeyTap(DWORD keyCode)
{
	if (!(prevKeyState[keyCode] & 0x80)
		&& keyState[keyCode] & 0x80)
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない

}

//任意のキーが離された瞬間かチェック
BOOL  Input::IsKeyRelease(DWORD keyCode)
{
	if ((prevKeyState[keyCode] & 0x80)
		&& !(keyState[keyCode] & 0x80))
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない

}

//マウスの任意のボタンが押されているかチェック
BOOL Input::IsMousePush(DWORD keyCode)
{
	if (mouseState.rgbButtons[keyCode] & 0x80)
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}


//マウスの任意のボタンが押された瞬間かチェック
BOOL  Input::IsMouseTap(DWORD keyCode)
{
	if (!(prevMouseState.rgbButtons[keyCode] & 0x80)
		&& mouseState.rgbButtons[keyCode] & 0x80)
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない

}

//マウスの任意のボタンがはなされた瞬間かチェック
BOOL  Input::IsMouseRelease(DWORD keyCode)
{
	if ((prevMouseState.rgbButtons[keyCode] & 0x80)
		&& !(mouseState.rgbButtons[keyCode] & 0x80))
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}

//マウスカーソルを中央に固定する関数
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
		return TRUE; //押してる
	}
	return FALSE;   //押してない
}

BOOL  Input::IsPadButtonTap(DWORD buttonCode, int ID)
{
	if (!(prevControllerState[ID].Gamepad.wButtons & buttonCode) &&
		controllerState[ID].Gamepad.wButtons & buttonCode)
	{
		return TRUE; //押してる
	}
	return FALSE;   //押してない
}

BOOL  Input::IsPadButtonRelease(DWORD buttonCode, int ID)
{
	if ((prevControllerState[ID].Gamepad.wButtons & buttonCode) &&
		!(controllerState[ID].Gamepad.wButtons & buttonCode))
	{
		return TRUE; //押してる
	}
	return FALSE;   //押してない
}

float Input::GetPadLStickX(int ID)
{
	float value = controllerState[ID].Gamepad.sThumbLX;

	//デッドゾーン
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

	//デッドゾーン
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

	//デッドゾーン
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

	//デッドゾーン
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

	//デッドゾーン
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

	//デッドゾーン
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
	vibration.wLeftMotorSpeed = leftSpeed * 65535; // 左モーター
	vibration.wRightMotorSpeed = rightSpeed * 65535; // 右モーター
	XInputSetState(ID, &vibration);
}