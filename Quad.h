#pragma once
#include "Global.h"

class Quad
{
	//�e�N�X�`���I�u�W�F�N�g
	LPDIRECT3DTEXTURE9 pTexture;

public:
	Quad();
	~Quad();
	HRESULT Load(char* fileName);
	HRESULT Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex = NULL);
};

