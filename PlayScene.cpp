/*===========================================================================================
�T�@���F�v���C�V�[������������
�쐬���F
�X�V���F
����ҁF
=============================================================================================*/

//---------------�C���N���[�h-----------------------
#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Invisible.h"

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�R���X�g���N�^
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
PlayScene::PlayScene()
{
	//���j�b�g��ǉ�
	unit.push_back(new Player);
	for (int i = 0; i < 10; i++)
	{
		unit.push_back(new Enemy);
	}
	unit.push_back(new Stage);
	unit.push_back(new Invisible);
}
