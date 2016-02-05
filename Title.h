#pragma once
#include "UnitBase.h"



class Title : public UnitBase
{

public:
	Title();
	~Title();
	HRESULT Load();
	HRESULT Update();
	HRESULT Render();
};