#include "Stage.h"



Stage::Stage()
{
	model = NULL;
	back = NULL;
}


Stage::~Stage()
{
	SAFE_DELETE(model);
	SAFE_DELETE(back);
}

HRESULT Stage::Load()
{
	model = new Fbx;
	model->Load("Assets\\models\\stage2.fbx");
	back = new Sprite;
	back->Load("Assets\\pictures\\Sky.jpg");
	return S_OK;
}

HRESULT Stage::Render()
{
	D3DXMATRIX matW;
	D3DXMatrixScaling(&matW,1,1,1);
	model->MatrixIn(&matW);
	model->Draw(&matW);

	SpriteData data;
	data.pos = D3DXVECTOR3(0, 0, 1);
	back->Draw(&data);

	//for (int i = 0;i < trees.)

	return S_OK;
}