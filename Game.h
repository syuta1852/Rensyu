/*===========================================================================================
�T�@���F�Q�[���S�̗̂������������
�쐬���F
�X�V���F
����ҁF
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include "SceneBase.h"

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�Q�[���N���X
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class Game
{
	//Direct3D�I�u�W�F�N�g
	LPDIRECT3D9 pD3d;

	//�V�[���I�u�W�F�N�g
	SceneBase* scene[SC_MAX];

	HRESULT Game::InitLight();

public:
	//�R���X�g���N�^
	Game();

	//�f�X�g���N�^
	~Game();

	//DirectX������
	HRESULT InitD3d(HWND);

	//�ǂݍ��ݏ���
	HRESULT Load();

	//�Փ˔��菈��
	HRESULT Hit();

	//�X�V����
	HRESULT Update();

	//�`�揈��
	HRESULT Render();
};

