#include "Tank.h"
#include "Camera.h"
#include "Stage.h"
#include "Wall.h"


Tank::Tank()
{
	//初期化
	body = NULL;
	head = NULL;
	position = D3DXVECTOR3(0, 0, 0);
	move = D3DXVECTOR3(0, 0, 0.5);
	dir = 0.0f;
	headDir = 0;
}


Tank::~Tank()
{
	SAFE_DELETE(head);
	SAFE_DELETE(body);
}

HRESULT Tank::Load()
{
	body = new Fbx;
	body->Load("Assets\\models\\tank_body.fbx");

	head = new Fbx;
	head->Load("Assets\\models\\tank_head.fbx");

	return S_OK;
}

HRESULT Tank::Render()
{
	D3DXMATRIX matW,matR,matT,matHR;
	D3DXMatrixTranslation(&matT,
						  position.x,
						  position.y,	//
						  position.z);	//Z軸
	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));
	D3DXMatrixRotationY(&matHR, D3DXToRadian(headDir));
	
	matW  = matR * matT;
	body->Draw(&matW);

	matW = matHR * matR * matT;
	head->Draw(&matW);
	return S_OK;
}
HRESULT Tank::Update()
{

	move = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX matR,matHR;
	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));
	D3DXVec3TransformCoord(&move, &move, &matR);

	//移動
	if (g_pInput->IsKeyPush(DIK_UP))
	{
		position += move;
	}

	if (g_pInput->IsKeyPush(DIK_DOWN))
	{
		position -= move;
	}
	if (g_pInput->IsKeyPush(DIK_LEFT))
	{
		dir -= 3.0f;
	}
	if (g_pInput->IsKeyPush(DIK_RIGHT))
	{
		dir += 3.0f;
	}
	if (g_pInput->IsKeyPush(DIK_Z))
	{
		headDir -= 3.0f;
	}
	if (g_pInput->IsKeyPush(DIK_X))
	{
		headDir += 3.0f;
	}
	
	g_pCamera->SetTarget(position);

	D3DXVECTOR3 camVec(0, 5, -8);
	D3DXMatrixRotationY(&matHR, D3DXToRadian(headDir));
	matHR *= matR;
	D3DXVec3TransformCoord(&camVec,&camVec, &matHR);

	g_pCamera->SetPos(position + camVec);

	return S_OK;
}
HRESULT Tank::Hit(UnitBase* pTarget)
{
	if (typeid(*pTarget) == typeid(Stage))
	{
		Stage* stage = (Stage*)pTarget;

		RayCastData ray;
		//レイが出る位置
		ray.start = position;
		ray.start.y = 50;

		//レイを向ける方向
		ray.dir = D3DXVECTOR3(0, -1, 0);

		//レイの情報を格納したアドレスを渡す
		stage->GetModel()->RayCast(&ray);

		if (ray.hit)
		{
			position = ray.pos;
		}
	}

	if (typeid(*pTarget) == typeid(Wall))
	{
		Wall* wall= (Wall*)pTarget;

		RayCastData ray;
		//レイが出る位置
		ray.start = position;
	

		//レイを向ける方向
		ray.dir = D3DXVECTOR3(0, 0, 1);
		
		//ray.dir = rayVec;
		//レイの情報を格納したアドレスを渡す
		wall->GetModel()->RayCast(&ray);

		if (ray.hit && ray.dist < 3)
		{
			position -= move;
		}
	}

	return S_OK;
}