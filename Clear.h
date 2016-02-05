#pragma once
#include "UnitBase.h"
#include "Sprite.h"
class Clear : public UnitBase
{
	Sprite* sprite;
public:
	Clear();
	~Clear();

	HRESULT Load();
	HRESULT Update();
	HRESULT Render();
};

