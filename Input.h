#pragma once
#define DIRECTINPUT_VERSION 0x800 //DirectInput �̃o�[�W�����ݒ�

#include <dinput.h>
#include <XInput.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"Xinput.lib")

#define SAFE_RELEASE(p) { if(p != NULL) { (p)->Release(); (p) = NULL; } }

class Input
{
	LPDIRECTINPUT8        pDinput; //DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8  pKeyDevice;  //�f�o�C�X�I�u�W�F�N�g�i�L�[�{�[�h�j
	LPDIRECTINPUTDEVICE8  pMouseDevice; //�f�o�C�X�I�u�W�F�N�g�i�}�E�X�j

	BYTE  keyState[256];    //�e�L�[�̏��
	BYTE  prevKeyState[256];//�O��̃L�[�̏��

	DIMOUSESTATE	mouseState;		//�}�E�X�̏��
	DIMOUSESTATE	prevMouseState;	//�O��̃}�E�X�̏��
	POINT			mousePos;		//�}�E�X�J�[�\���̈ʒu

	XINPUT_STATE    controllerState[4];   //�R���g���[���[�̏��
	XINPUT_STATE    prevControllerState[4];   //�O��̃R���g���[���[�̏��
	HWND			m_hWnd;

public:
	Input();
	~Input();
	HRESULT Init(HWND hWnd);//DirectInput�̏���
	HRESULT Update();       //�e���̓f�o�C�X�̏�Ԃ��擾

	BOOL  IsKeyPush(DWORD keyCode); //�C�ӂ̃L�[��������Ă��邩�`�F�b�N
	BOOL  IsKeyTap(DWORD keyCode); //�C�ӂ̃L�[�������ꂽ�u�Ԃ��`�F�b�N
	BOOL  IsKeyRelease(DWORD keyCode);

	//�}�E�X�֘A
	BOOL  IsMousePush(DWORD keyCode);		//�}�E�X�̔C�ӂ̃{�^����������Ă��邩�`�F�b�N
	BOOL  IsMouseTap(DWORD keyCode);		//�}�E�X�̔C�ӂ̃{�^���������ꂽ�u�Ԃ��`�F�b�N
	BOOL  IsMouseRelease(DWORD keyCode);	//�}�E�X�̔C�ӂ̃{�^�����͂Ȃ��ꂽ�u�Ԃ��`�F�b�N
	void  SetMousePos(POINT pos) { mousePos = pos; }	//�}�E�X�J�[�\���̈ʒu���Z�b�g
	POINT GetMousePos() { return mousePos; }			//�}�E�X�J�[�\���̈ʒu���擾
	void  SetCursorCenter();


	BOOL  IsPadButtonPush(DWORD buttonCode, int ID = 0);    //�R���g���[���[�̃{�^����������Ă��邩
	BOOL  IsPadButtonTap(DWORD buttonCode, int ID = 0);
	BOOL  IsPadButtonRelease(DWORD buttonCode, int ID = 0);

	float GetPadLStickX(int ID = 0);			//���X�e�B�b�N�̍��E�̌X��
	float GetPadLStickY(int ID = 0);			//���X�e�B�b�N�̏㉺�̌X��
	float GetPadRStickX(int ID = 0);			//�E�X�e�B�b�N�̍��E�̌X��
	float GetPadRStickY(int ID = 0);			//�E�X�e�B�b�N�̏㉺�̌X��
	float GetPadLTrigger(int ID = 0);			//���g���K�[�̉�������
	float GetPadRTrigger(int ID = 0);			//�E�g���K�[�̉�������

	void  Vibration(float leftSpeed, float rightSpeed, int ID = 0);                   //�U���G�t�F�N�g
};

