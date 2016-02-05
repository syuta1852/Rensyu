#include "Item.h"

//コンストラクタ
Item::Item()
{
	position = D3DXVECTOR3(-999, -999, -999);	//位置の初期化
	move = D3DXVECTOR3(0, 0, 5);		//移動量
	model = NULL;						//モデルデータのポインタの初期化
	isShot = FALSE;						//存在(発射)しているか
}

//デストラクタ
Item::~Item()
{
	SAFE_DELETE(model);	//モデルデータの解放
}
HRESULT Item::Load()
{
	model = new Fbx;							//アイテムのモデルデータの作成
	model->Load("Assets\\models\\player.fbx");	//FBXのロードを呼ぶ("現時点ではenemyで代用")
	return S_OK;
}
//描画
//引数1:プレイヤーの位置
HRESULT Item::Render()
{
	if (isShot)//真:存在 偽:存在していない
	{
		D3DXMATRIX matW;	// 行列の作成
		D3DXMatrixTranslation(&matW, position.x, position.y, position.z); // 弾が出るときのプレイヤーの位置を行列に
		model->Draw(&matW); //作った行列を基に描画
	}
	return S_OK;
}

//更新
//引数なし
HRESULT Item::Update()
{
	static int count = 0;
	if (isShot)//発射後なら
	{
		position += move;	//移動量分移動
		count++;			//カウントアップ
		if (count > 100)	//一定距離進んだら
		{
			count = 0;		//カウントを0に
			isShot = FALSE;	//未発射状態に
			position = D3DXVECTOR3(-999, -999, -999);
			move = D3DXVECTOR3(0, 0, 5);
		}
	}
	return S_OK;
}
//作成されたらフラグを立てるのとプレイヤーのポジションの格納
BOOL Item::Shot(D3DXVECTOR3 pos, D3DXMATRIX matW)
{
	if (!isShot)
	{
		position = pos;	//ポジションの格納
		D3DXVec3TransformCoord(&move, &move, &matW);
	}
	isShot = TRUE;	//発射状態に
	return TRUE;
}
BOOL Item::Hit(D3DXVECTOR3 enemyPos)
{
	D3DXVECTOR3 v = enemyPos - position;
	if (D3DXVec3Length(&v) < 2)
	{
		position = D3DXVECTOR3(-999, -999, -999);
		return TRUE;
	}
	return FALSE;
}