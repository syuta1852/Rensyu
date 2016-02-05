#pragma once
#include "UnitBase.h"
#include "Fbx.h"
#include "Item.h"

#define JUMP_POWER 200		//�W�����v�̗�
#define GRAVITY	   0.08f	//�d��
#define MAX_SPEED  0.5f		//�v���C���[�̍ő呬�x
#define FRICTION   0.01f	//���C��

class Player :public UnitBase
{
	//<----�e�N���X�̒�`---->
	Fbx*		player;			//���f���̃C���X�^���X���i�[����|�C���^�쐬
	Item*		item;			//�A�C�e���̃C���X�^���X���i�[����|�C���^�쐬
	//<----�ړ��֘A---->
	float		dir;			//���_��]�p��float�ϐ�
	float		dirY;			//���_��]�p(�c)��float�ϐ�
	D3DXVECTOR3 move;			//�ړ��p�x�N�g��
	//<----����x�N�g���֘A---->
	BOOL		isWallHit;		//�X�e�[�W�ƐڐG���Ă��邩
	D3DXVECTOR3 wallNormal;		//�X�e�[�W�̖@���̎擾�p

	//<----�W�����v�֘A---->
	BOOL		jump_flg;		//�W�����v�t���O
	int			jump_Count;		//�W�����v�J�E���g
	BOOL		ground_flg;		//�n�ʂɐڂ��Ă��邩

	//<----�����֘A---->
	
	D3DXVECTOR3 camVec;			//�J�������ړ����邽�߂̃x�N�g���̍쐬
	BOOL		cam;			//�J�����ƃX�e�[�W�̊ԂɃX�e�[�W�����邩�̐^�U�l
	
	//<----�}�E�X�֘A---->
	POINT		prevMousePos;	//�}�E�X�̑O�̈ʒu�̋L��
	BOOL		isRightCrick;	//�E�N���b�N�������ꂽ��

	//<----�����Ȃ�����---->
	BOOL		isInvisible;	//�����Ȃ�����ɂ��邩�ǂ���


public:
	Player();						//�R���X�g���N�^
	~Player();						//�f�X�g���N�^
	HRESULT Load();					//�v���C���[��fbx�f�[�^�Ȃǂ̃��[�h�֐�(�I�[�o�[���C�h)
	HRESULT Render();				//�v���C���[�̕`�揈������(�I�[�o�[���C�h)
	HRESULT Update();				//�v���C���[�̍X�V(1�b�Ԃ�60�t���[��,�I�[�o�[���C�h)
	HRESULT Hit(UnitBase* pTarget);	//�v���C���[�̓����蔻��p�֐�(�I�[�o�[���C�h)
	//�A�N�Z�X�֐�
	D3DXVECTOR3 GetPos(){ return position; }
	BOOL		GetInvisible(){ return isInvisible; }
	
};

