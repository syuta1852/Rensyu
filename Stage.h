#pragma once
#include "UnitBase.h"
#include "Fbx.h"
#include "Sprite.h"
#include "Quad.h"
#include <vector>

class Stage :	public UnitBase
{
	Fbx* model;
	Sprite* back;
	Quad* tree;
	
	struct TreeData
	{
		D3DXVECTOR3 pos;
		float dist;
	};
	std::vector<TreeData> trres;

	class TreeSort
	{
	public:
		bool operator()(const TreeData &a, const TreeData &b)
		{
			return a.dist < b.dist;
		}
	};

public:
	Stage();
	~Stage();
	HRESULT Load();
	HRESULT Render();
	//当たり判定に必要なアクセス関数
	Fbx* GetModel(){ return model; };
	D3DXVECTOR3 GetPos(){ return position; };
};

