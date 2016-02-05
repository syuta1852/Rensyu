#pragma once
#define DIRECTINPUT_VERSION 0x800 //DirectInput のバージョン設定

#include <dinput.h>
#include <XInput.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"Xinput.lib")

#define SAFE_RELEASE(p) { if(p != NULL) { (p)->Release(); (p) = NULL; } }

class Input
{
	LPDIRECTINPUT8        pDinput; //DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8  pKeyDevice;  //デバイスオブジェクト（キーボード）
	LPDIRECTINPUTDEVICE8  pMouseDevice; //デバイスオブジェクト（マウス）

	BYTE  keyState[256];    //各キーの状態
	BYTE  prevKeyState[256];//前回のキーの情報

	DIMOUSESTATE	mouseState;		//マウスの状態
	DIMOUSESTATE	prevMouseState;	//前回のマウスの状態
	POINT			mousePos;		//マウスカーソルの位置

	XINPUT_STATE    controllerState[4];   //コントローラーの状態
	XINPUT_STATE    prevControllerState[4];   //前回のコントローラーの状態
	HWND			m_hWnd;

public:
	Input();
	~Input();
	HRESULT Init(HWND hWnd);//DirectInputの準備
	HRESULT Update();       //各入力デバイスの状態を取得

	BOOL  IsKeyPush(DWORD keyCode); //任意のキーが押されているかチェック
	BOOL  IsKeyTap(DWORD keyCode); //任意のキーが押された瞬間かチェック
	BOOL  IsKeyRelease(DWORD keyCode);

	//マウス関連
	BOOL  IsMousePush(DWORD keyCode);		//マウスの任意のボタンが押されているかチェック
	BOOL  IsMouseTap(DWORD keyCode);		//マウスの任意のボタンが押された瞬間かチェック
	BOOL  IsMouseRelease(DWORD keyCode);	//マウスの任意のボタンがはなされた瞬間かチェック
	void  SetMousePos(POINT pos) { mousePos = pos; }	//マウスカーソルの位置をセット
	POINT GetMousePos() { return mousePos; }			//マウスカーソルの位置を取得
	void  SetCursorCenter();


	BOOL  IsPadButtonPush(DWORD buttonCode, int ID = 0);    //コントローラーのボタンが押されているか
	BOOL  IsPadButtonTap(DWORD buttonCode, int ID = 0);
	BOOL  IsPadButtonRelease(DWORD buttonCode, int ID = 0);

	float GetPadLStickX(int ID = 0);			//左スティックの左右の傾き
	float GetPadLStickY(int ID = 0);			//左スティックの上下の傾き
	float GetPadRStickX(int ID = 0);			//右スティックの左右の傾き
	float GetPadRStickY(int ID = 0);			//右スティックの上下の傾き
	float GetPadLTrigger(int ID = 0);			//左トリガーの押し込み
	float GetPadRTrigger(int ID = 0);			//右トリガーの押し込み

	void  Vibration(float leftSpeed, float rightSpeed, int ID = 0);                   //振動エフェクト
};

