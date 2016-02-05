//作成者:佐々木裕太
//更新日:11/2
#include "Enemy.h"
#include "Player.h"
#include "Stage.h"

//コンストラクタ
Enemy::Enemy()
{
	position = D3DXVECTOR3(0, 40, 0);			//位置の初期化
	enemy = NULL;								//enemyのポインタ初期化
	move = D3DXVECTOR3(0, 0, (rand()%10));				//移動ベクトルの初期化
	dir = 0;									//回転用変数の初期化
	targetPos = D3DXVECTOR3(-999, -999, -999);	//プレイヤー位置の初期化(ありえない数値に)
	timer = 0;
	count = 1;
	y_prev = 0;
	jump_flg = FALSE;
}
//デストラクタ
Enemy::~Enemy()
{
	SAFE_DELETE(enemy);	//enemyのDELETE
}

//Load関数
//引数:なし
//戻値:成功かどうか
HRESULT Enemy::Load()
{
	enemy = new Fbx;							//Fbxクラスのインスタンスを生成
	enemy->Load("Assets\\Models\\enemy.fbx");	//FbxデータのLoadを呼び出す
	return S_OK;
}
//Render関数
//引数:なし
//戻値:成功かどうか
HRESULT Enemy::Render()
{
	D3DXMATRIX matW,matR;	//描画に必要な行列の作成
	D3DXMatrixTranslation(&matW, position.x, position.y, position.z);	//描画位置をpositionへ
	D3DXMatrixRotationY(&matR,D3DXToRadian(dir));
	matW = matR * matW;
	/*static int count;*/
	if (state == STATE_HIT)
	{
		count++;
		if (count%10 == 0)
		{
			enemy->Draw(&matW);	//描画
			count = 0;
			
		}
	}
	else if (state == STATE_DEAD)
	{

	}
	else
	{
		enemy->Draw(&matW);	//描画
		count = 0;
	}
	return S_OK;
}
//Update関数
//引数:なし
//戻値:成功かどうか
HRESULT Enemy::Update()
{
	//タイマーの宣言
	//常にタイマーをカウント(一定の行動をしたら0になる)
	//stateの状態が変わるたびにリセットされる
	timer++;

	static int count = 0;

	//-----ベクトルの回転-----
	move = D3DXVECTOR3(0, move.y, 0.05);				//ベクトルの初期化
	D3DXMATRIX matR;								//回転行列作成用
	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));	//回転行列の中身を作成
	D3DXVec3TransformCoord(&move, &move, &matR);	//ベクトルと行列の掛け算

	switch (state)
	{
	case STATE_WALK:

		/*if (jump_flg)
		{
		float y_temp;
		y_temp = position.y;
		position.y -= (position.y - y_prev) + 1;
		y_prev = y_temp;
		if (position.y < 0)
		{
		jump_flg = FALSE;
		}

		}
		if (!jump_flg)
		{
		jump_flg = TRUE;
		y_prev = position.y;
		position.y += 30;
		}*/

		//前進
		position += move;
		if (rand() % 1000 == 0)
		{
			if (rand() % 2 == 0)
			{
				dir += 90;
				timer = 0;
			}
			else
			{
				dir -= 90;
				timer = 0;
			}
		}
		//一定時間動いたら回転
		if (100 < timer)
		{
			state = STATE_ROTA;
			timer = 0;
		}
		if (Search() && !isInvisible)
		{
			state = STATE_RUN;
		}
		break;
	case STATE_ROTA:
		if (rand() % 2 == 0 && isWallHit)
		{
			dir += 90;
			state = STATE_WALK;
		}
		else if (rand()%2 == 1 && isWallHit)
		{
			dir -= 90;
			state = STATE_WALK;
		}
		if (!isWallHit)
		{
			dir += 180;
			state = STATE_WALK;
			timer = 0;
		}
		
		
		break;
	case STATE_HIT:
		if (100 < timer)
		{
			state = STATE_WALK;
			timer = 0;
		}
		break;

	case STATE_RUN:
	{
		
		  //追跡中
		  position += move * 2;

		  D3DXVECTOR3 pToDist = targetPos - position;
		  D3DXVECTOR3 cross;//外積格納用
		  D3DXVec3Cross(&cross, &move, &pToDist);

		  //左右どちらにいるか
		  if (cross.y < 0)
		  {
			  dir += -5;
		  }
		  else
		  {
			  dir += 5;
		  }
		  if (isInvisible)
		  {
			  state = STATE_WALK;
		  }

		  if (Search() == FALSE)
		  {
			  state = STATE_WALK;
			  timer = 0;
		  }
		  break;
	}

	}
	return S_OK;
}
HRESULT Enemy::Hit(UnitBase* pTarget)
{
	//ステージとの当たり判定
	if (typeid(*pTarget) == typeid(Stage))
	{
		Stage* stage = (Stage*)pTarget;

		RayCastData ray;
		//レイが出る位置
		ray.start = position;
		ray.start.y = position.y +10;

		//レイを向ける方向
		ray.dir = D3DXVECTOR3(0, -1, 0);

		//レイの情報を格納したアドレスを渡す
		stage->GetModel()->RayCast(&ray);
		if (ray.hit)
		{
			position = ray.pos + D3DXVECTOR3(0,0.5,0);
		}

		ray.start = position;
		D3DXMATRIX matR;
		D3DXMatrixRotationY(&matR, D3DXToRadian(dir));
		D3DXVECTOR3 rayVec = D3DXVECTOR3(0, 0, 1);
		D3DXVec3TransformCoord(&rayVec, &rayVec, &matR);
		ray.dir = rayVec;
		stage->GetModel()->RayCast(&ray);
		if (ray.hit && ray.dist <= 2)
		{
			isWallHit = TRUE;
			state = STATE_ROTA;
		}
	}

	if (typeid(*pTarget) == typeid(Player))
	{
		Player* player = (Player*)pTarget;
		//プレイヤーの位置を保存
		targetPos = player->GetPos();
		isInvisible = player->GetInvisible();
	}
	return S_OK;
}

BOOL Enemy::Search()
{
	//プレイヤーの位置-自分の位置でプレイヤーに向かう長さのわからないベクトルを求める
	D3DXVECTOR3 pToDist = targetPos - position;	
	float pDist = D3DXVec3Length(&pToDist);	//ベクトルの長さを測り変数に格納

	//今移動している方向へのベクトルと
	//プレイヤーの位置へのベクトルを正規化(内積で角度を求めるため)
	D3DXVec3Normalize(&move,&move);			//今移動しているベクトル
	D3DXVec3Normalize(&pToDist, &pToDist);	//プレイヤー位置へのベクトル

	//内積で角度を求める(Dot関数の戻値はCOSのためacos関数で度に直す必要あり)
	float d = D3DXVec3Dot(&move, &pToDist);
	//コサイン値を度数に変換
	d = acos(d);

	//ベクトルの長さと角度を使って見えているか判定
	if (pDist <= 20 && d <= D3DXToRadian(40))
	{
		return TRUE;
	}

	return FALSE;
}

//Enemyをヒット状態に
void Enemy::KnockBack()
{
	if (state != STATE_RUN)
	{
		state = STATE_HIT;
		timer = 0;
	}
}

//Enemyを倒したときの処理
void Enemy::Kill()
{
	state = STATE_DEAD;
	position = D3DXVECTOR3(-999, -999, 0);
}