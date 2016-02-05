#pragma once
#include "UnitBase.h"
#include "Fbx.h"

class Wall :
	public UnitBase
{
	Fbx* model;
public:
	Wall();
	~Wall();
	HRESULT Load();
	HRESULT Render();
	Fbx* GetModel(){ return model;}
};

