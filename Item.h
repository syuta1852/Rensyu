#pragma once
#include "UnitBase.h"
#include "Fbx.h"
class Item
{
	Fbx* model;				//���f���f�[�^
	D3DXVECTOR3 position;	//�ʒu���
	D3DXVECTOR3 move;		//�ړ���
	BOOL isShot;			//���˂��ꂽ���ǂ���
public:
	Item();						//�R���X�g���N�^
	~Item();					//�f�X�g���N�^
	HRESULT Load();				//�ǂݍ���
	HRESULT Render();			//�`��
	HRESULT Update();			//�X�V
	BOOL Hit(D3DXVECTOR3 enemyPos);	//�e�����������Ƃ��̏���
	BOOL Shot(D3DXVECTOR3 pos,D3DXMATRIX matW); //�A�C�e���̔���
};

