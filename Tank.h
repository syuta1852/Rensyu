#pragma once
#include "UnitBase.h"
#include "Fbx.h"

class Tank : public UnitBase
{
	Fbx *body, *head;
	D3DXVECTOR3 move;
	float dir;
	float headDir;

public:
	Tank();
	~Tank();
	HRESULT Load();
	HRESULT Render();
	HRESULT Update();
	HRESULT Hit(UnitBase* pTarget);
};

