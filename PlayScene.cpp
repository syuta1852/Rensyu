/*===========================================================================================
概　略：プレイシーンを扱う処理
作成日：
更新日：
制作者：
=============================================================================================*/

//---------------インクルード-----------------------
#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Invisible.h"

//――――――――――――――――――――――
//	コンストラクタ
//――――――――――――――――――――――
PlayScene::PlayScene()
{
	//ユニットを追加
	unit.push_back(new Player);
	for (int i = 0; i < 10; i++)
	{
		unit.push_back(new Enemy);
	}
	unit.push_back(new Stage);
	unit.push_back(new Invisible);
}
