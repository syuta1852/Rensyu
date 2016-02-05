#include "Clear.h"


Clear::Clear()
{
	sprite = NULL;
}


Clear::~Clear()
{
	SAFE_DELETE(sprite);
}
HRESULT Clear::Load()
{
	sprite->Load("Assets\\pictures\\clear.bmp");
	return S_OK;
}

HRESULT Clear::Update()
{

	return S_OK;
}

HRESULT Clear::Render()
{
	SpriteData data;
	sprite->Draw(&data);
	return S_OK;
}