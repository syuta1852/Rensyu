#pragma once
#include "UnitBase.h"
#include "Fbx.h"

class Invisible : public UnitBase
{
	Fbx* model;
public:
	Invisible();
	~Invisible();
	HRESULT Load();
	HRESULT Render();
	Fbx* GetModel(){ return model; };
	D3DXVECTOR3 GetPos(){ return position; };
};