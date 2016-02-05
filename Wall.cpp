#include "Wall.h"


Wall::Wall()
{
	model = NULL;
}


Wall::~Wall()
{
	SAFE_DELETE(model);
}

HRESULT Wall::Load()
{
	model = new Fbx;
	model->Load("Assets\\models\\Wall.fbx");
	return S_OK;
}

HRESULT Wall::Render()
{
	D3DXMATRIX matW;
	D3DXMatrixIdentity(&matW);

	model->Draw(&matW);
	return S_OK;
}