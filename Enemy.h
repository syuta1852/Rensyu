#include "UnitBase.h"	//UnitBaseクラスヘッダー
#include "Fbx.h"		//Fbxクラスヘッダー

//UnitBaseクラスを継承したEnemyクラスの作成
class Enemy :public UnitBase
{
	Fbx*		enemy;		//Fbxクラスのポインタ
	D3DXVECTOR3 move;		//移動用のベクトル
	float		dir;		//回転と視点の角度
	D3DXVECTOR3 WallNormal;	//壁の法線を格納するための変数
	BOOL		isWallHit;	//壁と当たっているかの判定用
	D3DXVECTOR3 targetPos;	//見つけたときにプレイヤーの位置を格納するための変数
	float		wallDist;	//プレイヤーの位置と壁の位置のどちらが近いかの判定用
	DWORD		state;		//現在の状態を格納するための変数
	DWORD		timer;
	int			count;
	float		y_prev;		//Y軸の元の座標値を入力
	BOOL		jump_flg;	//ジャンプしているか
	BOOL		isInvisible;//プレイヤーが見えない場所にいるか

	//状態
	enum
	{
		STATE_ROTA,		//回転
		STATE_WALK,		//前進状態
		STATE_HIT,		//弾に当たったときの処理
		STATE_DEAD,		//倒されたときの処理
		STATE_RUN,		//追跡
	};


public:
	Enemy();									//コンストラクタ
	~Enemy();									//デストラクタ
	HRESULT Load();								//Load関数(オーバーライド)
	HRESULT Render();							//Render関数(オーバーライド)
	HRESULT Update();							//Update関数(オーバーライド)
	HRESULT Hit(UnitBase* pTarget);				//Hit関数(オーバーライド)
	BOOL	Search();							//敵を見つけているかの判定関数
	D3DXVECTOR3 GetPos(){ return position; }	//ポジションのアクセス関数

	void KnockBack();	//弾が当たった状態に変更する関数
	void Kill();		//弾が当たった状態でプレイヤーに接触したときの処理関数
};
