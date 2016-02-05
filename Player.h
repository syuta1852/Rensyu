#pragma once
#include "UnitBase.h"
#include "Fbx.h"
#include "Item.h"

#define JUMP_POWER 200		//ジャンプの力
#define GRAVITY	   0.08f	//重力
#define MAX_SPEED  0.5f		//プレイヤーの最大速度
#define FRICTION   0.01f	//摩擦力

class Player :public UnitBase
{
	//<----各クラスの定義---->
	Fbx*		player;			//モデルのインスタンスを格納するポインタ作成
	Item*		item;			//アイテムのインスタンスを格納するポインタ作成
	//<----移動関連---->
	float		dir;			//視点回転用のfloat変数
	float		dirY;			//視点回転用(縦)のfloat変数
	D3DXVECTOR3 move;			//移動用ベクトル
	//<----滑りベクトル関連---->
	BOOL		isWallHit;		//ステージと接触しているか
	D3DXVECTOR3 wallNormal;		//ステージの法線の取得用

	//<----ジャンプ関連---->
	BOOL		jump_flg;		//ジャンプフラグ
	int			jump_Count;		//ジャンプカウント
	BOOL		ground_flg;		//地面に接しているか

	//<----慣性関連---->
	
	D3DXVECTOR3 camVec;			//カメラを移動するためのベクトルの作成
	BOOL		cam;			//カメラとステージの間にステージがあるかの真偽値
	
	//<----マウス関連---->
	POINT		prevMousePos;	//マウスの前の位置の記憶
	BOOL		isRightCrick;	//右クリックが押されたか

	//<----見えない足場---->
	BOOL		isInvisible;	//見えない足場にいるかどうか


public:
	Player();						//コンストラクタ
	~Player();						//デストラクタ
	HRESULT Load();					//プレイヤーのfbxデータなどのロード関数(オーバーライド)
	HRESULT Render();				//プレイヤーの描画処理周り(オーバーライド)
	HRESULT Update();				//プレイヤーの更新(1秒間に60フレーム,オーバーライド)
	HRESULT Hit(UnitBase* pTarget);	//プレイヤーの当たり判定用関数(オーバーライド)
	//アクセス関数
	D3DXVECTOR3 GetPos(){ return position; }
	BOOL		GetInvisible(){ return isInvisible; }
	
};

