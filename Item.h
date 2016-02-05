#pragma once
#include "UnitBase.h"
#include "Fbx.h"
class Item
{
	Fbx* model;				//モデルデータ
	D3DXVECTOR3 position;	//位置情報
	D3DXVECTOR3 move;		//移動量
	BOOL isShot;			//発射されたかどうか
public:
	Item();						//コンストラクタ
	~Item();					//デストラクタ
	HRESULT Load();				//読み込み
	HRESULT Render();			//描画
	HRESULT Update();			//更新
	BOOL Hit(D3DXVECTOR3 enemyPos);	//弾が当たったときの処理
	BOOL Shot(D3DXVECTOR3 pos,D3DXMATRIX matW); //アイテムの発射
};

