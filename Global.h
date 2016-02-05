/*===========================================================================================
�T�@���F�v���W�F�N�g�S�̂ŕK�v�Ȃ��̂��܂Ƃ߂��t�@�C��
�쐬���F
�X�V���F
����ҁF
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include <windows.h>
#include <d3dx9.h>
#include "Input.h"

//----------���C�u�����t�@�C���̃��[�h--------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//-------------------�萔�錾------------------
//�E�B���h�E�T�C�Y
#define WINDOW_WIDTH	800	//�E�B���h�E�̕�
#define WINDOW_HEIGHT	480		//�E�B���h�E����

#define ACTOR_SIZE		60

//-------------------�}�N��------------------
#define SAFE_DELETE(p)     {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p) {delete[] (p);  (p) = NULL;}
#define SAFE_RELEASE(p) { if(p != NULL) { (p)->Release(); (p) = NULL; } }

//-------------------�O���[�o���ϐ�------------------
//�V�[��
enum GAME_SCENE
{
	SC_TITLE,		//�^�C�g���V�[��
	SC_PLAY,		//�v���C�V�[��
	SC_CLEAR,		//�N���A�V�[��
	SC_GAME_OVER,	//�Q�[���I�[�o�[�V�[��
	SC_MAX
};
extern GAME_SCENE	g_gameScene;		//���Ԃ�Game.cpp�Ő錾

//Direct3D�f�o�C�X�I�u�W�F�N�g
extern LPDIRECT3DDEVICE9 g_pDevice;

//���͏����I�u�W�F�N�g
extern Input* g_pInput;

//�J�����I�u�W�F�N�g
class Camera;
extern Camera* g_pCamera;
