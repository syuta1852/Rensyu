#pragma once

#include "Global.h"


//�摜�̕\���Ɏg���f�[�^
struct SpriteData
{
	D3DXVECTOR3 pos;      //�ʒu
	D3DXVECTOR2 size;     //�T�C�Y
	D3DXVECTOR2 cut;      //�؂蔲���ʒu
	D3DXVECTOR2 center;   //��_
	D3DXVECTOR2 scale;    //�g�嗦
	float		angle;	  //�p�x

	//�F
	struct
	{
		int a, r, g, b;
	}color;

	//�R���X�g���N�^
	SpriteData()
	{
		pos =	D3DXVECTOR3(0, 0, 0);
		size =	D3DXVECTOR2(-999, -999);
		cut =	D3DXVECTOR2(0, 0);
		center = D3DXVECTOR2(-999, -999);
		color = { 255, 255, 255, 255 };
		scale = D3DXVECTOR2(1, 1);
		angle = 0;
	}
};


class Sprite
{
	//�X�v���C�g�I�u�W�F�N�g
	LPD3DXSPRITE pSprite;

	//�e�N�X�`���I�u�W�F�N�g
	LPDIRECT3DTEXTURE9 pTexture;

	//�e�N�X�`���̃T�C�Y
	D3DXVECTOR2  texSize;

public:
	Sprite();
	~Sprite();

	HRESULT Load(char* fileName);
	void Draw(SpriteData* data);
};