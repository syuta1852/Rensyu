#include "Invisible.h"
Invisible::Invisible()
{
	position = D3DXVECTOR3(-20,1,0);
	model = NULL;
}


Invisible::~Invisible()
{
	SAFE_DELETE(model);
}

HRESULT Invisible::Load()
{
	model = new Fbx;
	model->Load("Assets\\models\\Clear.fbx");
	return S_OK;
}

HRESULT Invisible::Render()
{
	D3DXMATRIX matW;
	D3DXMatrixTranslation(&matW, position.x, position.y, position.z);
	model->Draw(&matW);


	return S_OK;
}