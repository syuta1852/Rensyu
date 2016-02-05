/*===========================================================================================
概　略：各シーンの親クラス
作成日：
更新日：
制作者：
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include <vector>
#include "UnitBase.h"

//―――――――――――――――――――――
//	各シーンの親クラス
//―――――――――――――――――――――
class SceneBase
{
protected:
	//シーンに登場するユニット
	std::vector<UnitBase*> unit;

public:
	//デストラクタ
	virtual ~SceneBase();

	//読み込み処理
	HRESULT Load();

	//衝突判定処理
	HRESULT Hit();

	//更新処理
	HRESULT Update();

	//描画処理
	HRESULT Render();
};

