#include "Player.h"		//プレイヤークラスのヘッダーのインクルード
#include "Camera.h"		//カメラの位置の調整用
#include "Stage.h"		//ステージとの当たり判定に必要
#include "Enemy.h"		//敵との当たり判定に必要
#include "Invisible.h"	//見えない足場用

//コンストラクタ
Player::Player()
{
	player = NULL;						//Fbxクラスのポインタの初期化
	position = D3DXVECTOR3(-30, 0, 0);	//初期位置へ移動
	move = D3DXVECTOR3(0, 0, 0);		//移動用のベクトルの初期化
	dir = 90.0f;						//視点回転用の変数の初期化
	jump_flg = TRUE;					//ジャンプしているかの判定
	jump_Count = 0;						//ジャンプの回数
	ground_flg = FALSE;					//地面にいるかの判定
	item = NULL;						//アイテムのポインタの初期化
	camVec = D3DXVECTOR3(0, 3, -8);		//カメラ移動用のベクトル
	isRightCrick = FALSE;				//右クリックの判定用の変数初期化
	isInvisible = FALSE;				//見えない地面の判定用の変数初期化
	isWallHit = FALSE;					//ステージに当たっていない状態に
	wallNormal = D3DXVECTOR3(0, 0, 0);	//ステージの法線の初期化
}

//デストラクタ
Player::~Player()
{
	SAFE_DELETE(player);			//動的に作成したFbxクラスのオブジェクトを解放
	SAFE_DELETE(item)				//アイテムクラスのデリート
}

//Load関数
//引数:なし
//戻り値:成功かどうか
//機能:各データのロード
HRESULT Player::Load()
{
	player = new Fbx;										//Fbxクラスのインスタンスを作成
	if (FAILED(player->Load("assets\\Models\\player.fbx"))) //fbxクラスのロード
	{
		//ロードできなかったときの処理
		MessageBox(0, "playerデータのロードに失敗しました", "Player.cpp参照", MB_OK);
		//失敗を返す
		return E_FAIL;
	}

	g_pInput->SetCursorCenter();		//カーソルを中央へ（コンストラクタで呼ぶと、まだウィンドウができてないのでエラーになる）
	GetCursorPos(&prevMousePos);		//カーソルの位置を覚えておく
	ShowCursor(FALSE);					//マウスカーソルを表示しない


	
	item = new Item;	//アイテムのインスタンスを呼ぶ
	item->Load();		//アイテムのロードを呼ぶ
	return S_OK;		//成功を返す
}

//Render関数
//引数:なし
//戻り値:成功かどうか
//機能:プレイヤーの描画
HRESULT Player::Render()
{
	D3DXMATRIX matW, matR, matT;					//各行列の作成
	D3DXMatrixTranslation(&matT,position.x,			//移動行列の作成
							position.y,	position.z);
	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));  //回転行列の作成
	matW = matR * matT;								//行列の掛け算
	if (!isRightCrick)
	{
		player->Draw(&matW);							//プレイヤーの描画(引数は移動行列と回転行列をかけた行列)
	}

	//アイテムのrender
	item->Render();
	return S_OK;									//すべての処理に成功したら
}

//Update関数
//引数:なし
//戻り値:成功かどうか
//機能:プレイヤーのフレームごとの処理(移動など)
HRESULT Player::Update()
{
	POINT		mousePos;		//マウスの現在の位置の記憶
	GetCursorPos(&mousePos);	//現在のマウス位置格納

	//今のマウスの位置から元のマウスの位置(画面の中央に固定)を引いて差分を求める
	dir += (float)(mousePos.x - prevMousePos.x)/10;		
	dirY += (float)(mousePos.y-prevMousePos.y)/10;

	//制限（高さ方向は±30°）
	if (dirY < -30)	dirY = -30;
	if (dirY > 30)	dirY = 30;
	if (isRightCrick)
	{
		if (dirY > 0) dirY = 0;
	}

	g_pInput->SetCursorCenter();	//マウスカーソルを中心に
	

	move = D3DXVECTOR3(0, 0, 0.1);	//前後左右のベクトルを初期化(回転したときのため)
	D3DXMATRIX matR,matRY;				//回転行列を作成するための変数の作成
	static int count = 0;				//重力をかけるための秒数カウント用

	if (!cam && isRightCrick == FALSE)
	{
		//カメラのベクトルの初期化(カメラとプレイヤーの間にステージがなければ)
		camVec = D3DXVECTOR3(0, 5, -5);
	}
	if (g_pInput->IsMouseTap(1))
	{
		if (isRightCrick)
		{
			isRightCrick = FALSE;
		}
		else
		{
			isRightCrick = TRUE;
		}
	}

	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));	//回転行列の作成
	D3DXVec3TransformCoord(&move, &move, &matR);	//行列とベクトルの掛け算
	//---------------移動処理------------------------------------//
	//<--前方向に移動(↑キー入力)-->
	if (g_pInput->IsKeyPush(DIK_W))
	{
		if (isWallHit)
		{
			//ステージに当たっていたら滑りベクトルを使う
			position += move - (D3DXVec3Dot(&move, &wallNormal) * wallNormal);
		}
		else
		{
			//前へ移動
			position += move;
		}
			
	}
	//<--後ろ方向に移動(↓キー入力)-->
	else if (g_pInput->IsKeyTap(DIK_F))
	{
		dir += 180;	//視点半回転
	}

	//----------------------------------------------------------

	//<----ジャンプ処理---->
	if ((g_pInput->IsKeyTap(DIK_SPACE)))
	{
		move.y += 50;
	}
	
	if (move.y > 0)
	{
		move.y -= 0.5f * GRAVITY * count * count * 0.01f * count;
		count++;
	}
	if (move.y < 0)
	{
		move.y = 0;
		count = 0;
	}
	//--------------------------------------------------------------//


	D3DXMatrixRotationX(&matRY, D3DXToRadian(dirY));	//回転行列の作成
	matR = matRY*matR;									//横方向と縦方向の行列を掛ける

	//----------アイテム(飛び道具を飛ばす)処理----------------------//
	if (isRightCrick)
	{
		if (g_pInput->IsMousePush(0))
		{
			item->Shot(position, matR);
		}
	}
	
	//アイテムのアップデート
	item->Update();
	//--------------------------------------------------------------//


	//-----------------カメラ関連---------------------------------//
	

	D3DXVec3TransformCoord(&camVec, &camVec, &matR);	//行列とベクトルの掛け算
	D3DXVECTOR3 v(0, 0, 5);
	D3DXVec3TransformCoord(&v, &v, &matR);

	if (isRightCrick)
	{
		g_pCamera->SetTarget(position + v);				//カメラのターゲットをプレイヤーに設定
		g_pCamera->SetPos(position);					//カメラを一人称視点に
	}
	else
	{
		g_pCamera->SetTarget(position);					//カメラのターゲットをプレイヤーに設定
		g_pCamera->SetPos(position + camVec);			//カメラのpositionに作成した移動行列をプラスする
	}

	
	//マウスカーソルを中心へ
	g_pInput->SetCursorCenter();

	//その位置を記憶
	GetCursorPos(&prevMousePos);

	//------------------------------------------------------------//

	//成功を返す
	return S_OK;
}
//playerのHit関数
//引数:UnitBase型のポインタ
//(UnitBaseクラスを継承した子クラスの情報が扱える)
//戻り値:HRESULT型-成功かどうか
HRESULT Player::Hit(UnitBase* pTarget)
{
	//ステージとの当たり判定
	if (typeid(*pTarget) == typeid(Stage))
	{
		//pTargetをステージと判断し、stage型にキャスト
		Stage* stage = (Stage*)pTarget;

		//RayCastData型の変数を作る
		RayCastData ray;
		//レイが出る位置
		ray.start = position;
		ray.start.y = position.y+10;

		//レイを向ける方向
		ray.dir = D3DXVECTOR3(0, -1, 0);
		
		//レイの情報を格納したアドレスを渡す
		stage->GetModel()->RayCast(&ray);

		//Rayが当たっているかつ
		//ステージの位置Yがプレイヤー位置Y-1より大きかったら
		if (ray.hit && ray.pos.y >= (position.y - 1) && ground_flg == FALSE)
		{
			jump_flg = TRUE;
			jump_Count = 0;
			ground_flg = TRUE;
			position = ray.pos + D3DXVECTOR3(0, 0.5, 0);
		}
		else if (ground_flg == TRUE)
		{
			position = ray.pos + D3DXVECTOR3(0,0.5,0);
		}

		//目の前にステージがあるかの判定
		ray.start = position;
		D3DXMATRIX matR;
		D3DXMatrixRotationY(&matR, D3DXToRadian(dir));
		D3DXVECTOR3 rayVec = D3DXVECTOR3(0, 0, 1);
		D3DXVec3TransformCoord(&rayVec, &rayVec, &matR);
		ray.dir = rayVec;
		stage->GetModel()->RayCast(&ray);
		if (ray.hit && ray.dist < 2)
		{
				isWallHit = TRUE;
				wallNormal = ray.normal;
		}
		else
		{
			isWallHit = FALSE;
		}

		ray.start = position;
		ray.dir = D3DXVECTOR3(0, 0, -1);
		stage->GetModel()->RayCast(&ray);
		D3DXVECTOR3 v = stage->GetPos() - position;
		//プレイヤーとカメラの間にステージがあるか
		D3DXVECTOR3 pToCdist = camVec-position;
		if (D3DXVec3Length(&v) < D3DXVec3Length(&pToCdist))
		{
			camVec = D3DXVECTOR3(position.x, 5, position.z);
		}

	}
	//敵との当たり判定
	if (typeid(*pTarget) == typeid(Enemy))
	{
		static int count = 0;
		count--;
		Enemy* enemy = (Enemy*)pTarget;				//UnitBase型をEnemy型にキャスト
		BOOL flg = item->Hit(enemy->GetPos());		//アイテムが当たったかどうか
		if (flg)
		{
			enemy->KnockBack();
			count = 100;
		}
		D3DXVECTOR3 v = enemy->GetPos() - position; //プレイヤーから敵に向かうベクトルを求める
		if (D3DXVec3Length(&v) < 2)					//ベクトルの長さが2以下だったら
		{
			if (count >= 0)
			{
				enemy->Kill();
				g_gameScene = SC_CLEAR;
			}
			else
			{
				g_gameScene = SC_GAME_OVER;
			}
		}
	}
	if (typeid(*pTarget) == typeid(Invisible))
	{
		Invisible* invisible = (Invisible*)pTarget;
		D3DXVECTOR3 inTopVec = invisible->GetPos() - position;
		if (D3DXVec3Length(&inTopVec) < 1)
		{
			isInvisible = TRUE;
		}
		else
		{
			isInvisible = FALSE;
		}
	}



	return S_OK;
}



