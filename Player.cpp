#include "Player.h"		//�v���C���[�N���X�̃w�b�_�[�̃C���N���[�h
#include "Camera.h"		//�J�����̈ʒu�̒����p
#include "Stage.h"		//�X�e�[�W�Ƃ̓����蔻��ɕK�v
#include "Enemy.h"		//�G�Ƃ̓����蔻��ɕK�v
#include "Invisible.h"	//�����Ȃ�����p

//�R���X�g���N�^
Player::Player()
{
	player = NULL;						//Fbx�N���X�̃|�C���^�̏�����
	position = D3DXVECTOR3(-30, 0, 0);	//�����ʒu�ֈړ�
	move = D3DXVECTOR3(0, 0, 0);		//�ړ��p�̃x�N�g���̏�����
	dir = 90.0f;						//���_��]�p�̕ϐ��̏�����
	jump_flg = TRUE;					//�W�����v���Ă��邩�̔���
	jump_Count = 0;						//�W�����v�̉�
	ground_flg = FALSE;					//�n�ʂɂ��邩�̔���
	item = NULL;						//�A�C�e���̃|�C���^�̏�����
	camVec = D3DXVECTOR3(0, 3, -8);		//�J�����ړ��p�̃x�N�g��
	isRightCrick = FALSE;				//�E�N���b�N�̔���p�̕ϐ�������
	isInvisible = FALSE;				//�����Ȃ��n�ʂ̔���p�̕ϐ�������
	isWallHit = FALSE;					//�X�e�[�W�ɓ������Ă��Ȃ���Ԃ�
	wallNormal = D3DXVECTOR3(0, 0, 0);	//�X�e�[�W�̖@���̏�����
}

//�f�X�g���N�^
Player::~Player()
{
	SAFE_DELETE(player);			//���I�ɍ쐬����Fbx�N���X�̃I�u�W�F�N�g�����
	SAFE_DELETE(item)				//�A�C�e���N���X�̃f���[�g
}

//Load�֐�
//����:�Ȃ�
//�߂�l:�������ǂ���
//�@�\:�e�f�[�^�̃��[�h
HRESULT Player::Load()
{
	player = new Fbx;										//Fbx�N���X�̃C���X�^���X���쐬
	if (FAILED(player->Load("assets\\Models\\player.fbx"))) //fbx�N���X�̃��[�h
	{
		//���[�h�ł��Ȃ������Ƃ��̏���
		MessageBox(0, "player�f�[�^�̃��[�h�Ɏ��s���܂���", "Player.cpp�Q��", MB_OK);
		//���s��Ԃ�
		return E_FAIL;
	}

	g_pInput->SetCursorCenter();		//�J�[�\���𒆉��ցi�R���X�g���N�^�ŌĂԂƁA�܂��E�B���h�E���ł��ĂȂ��̂ŃG���[�ɂȂ�j
	GetCursorPos(&prevMousePos);		//�J�[�\���̈ʒu���o���Ă���
	ShowCursor(FALSE);					//�}�E�X�J�[�\����\�����Ȃ�


	
	item = new Item;	//�A�C�e���̃C���X�^���X���Ă�
	item->Load();		//�A�C�e���̃��[�h���Ă�
	return S_OK;		//������Ԃ�
}

//Render�֐�
//����:�Ȃ�
//�߂�l:�������ǂ���
//�@�\:�v���C���[�̕`��
HRESULT Player::Render()
{
	D3DXMATRIX matW, matR, matT;					//�e�s��̍쐬
	D3DXMatrixTranslation(&matT,position.x,			//�ړ��s��̍쐬
							position.y,	position.z);
	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));  //��]�s��̍쐬
	matW = matR * matT;								//�s��̊|���Z
	if (!isRightCrick)
	{
		player->Draw(&matW);							//�v���C���[�̕`��(�����͈ړ��s��Ɖ�]�s����������s��)
	}

	//�A�C�e����render
	item->Render();
	return S_OK;									//���ׂĂ̏����ɐ���������
}

//Update�֐�
//����:�Ȃ�
//�߂�l:�������ǂ���
//�@�\:�v���C���[�̃t���[�����Ƃ̏���(�ړ��Ȃ�)
HRESULT Player::Update()
{
	POINT		mousePos;		//�}�E�X�̌��݂̈ʒu�̋L��
	GetCursorPos(&mousePos);	//���݂̃}�E�X�ʒu�i�[

	//���̃}�E�X�̈ʒu���猳�̃}�E�X�̈ʒu(��ʂ̒����ɌŒ�)�������č��������߂�
	dir += (float)(mousePos.x - prevMousePos.x)/10;		
	dirY += (float)(mousePos.y-prevMousePos.y)/10;

	//�����i���������́}30���j
	if (dirY < -30)	dirY = -30;
	if (dirY > 30)	dirY = 30;
	if (isRightCrick)
	{
		if (dirY > 0) dirY = 0;
	}

	g_pInput->SetCursorCenter();	//�}�E�X�J�[�\���𒆐S��
	

	move = D3DXVECTOR3(0, 0, 0.1);	//�O�㍶�E�̃x�N�g����������(��]�����Ƃ��̂���)
	D3DXMATRIX matR,matRY;				//��]�s����쐬���邽�߂̕ϐ��̍쐬
	static int count = 0;				//�d�͂������邽�߂̕b���J�E���g�p

	if (!cam && isRightCrick == FALSE)
	{
		//�J�����̃x�N�g���̏�����(�J�����ƃv���C���[�̊ԂɃX�e�[�W���Ȃ����)
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

	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));	//��]�s��̍쐬
	D3DXVec3TransformCoord(&move, &move, &matR);	//�s��ƃx�N�g���̊|���Z
	//---------------�ړ�����------------------------------------//
	//<--�O�����Ɉړ�(���L�[����)-->
	if (g_pInput->IsKeyPush(DIK_W))
	{
		if (isWallHit)
		{
			//�X�e�[�W�ɓ������Ă����犊��x�N�g�����g��
			position += move - (D3DXVec3Dot(&move, &wallNormal) * wallNormal);
		}
		else
		{
			//�O�ֈړ�
			position += move;
		}
			
	}
	//<--�������Ɉړ�(���L�[����)-->
	else if (g_pInput->IsKeyTap(DIK_F))
	{
		dir += 180;	//���_����]
	}

	//----------------------------------------------------------

	//<----�W�����v����---->
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


	D3DXMatrixRotationX(&matRY, D3DXToRadian(dirY));	//��]�s��̍쐬
	matR = matRY*matR;									//�������Əc�����̍s����|����

	//----------�A�C�e��(��ѓ�����΂�)����----------------------//
	if (isRightCrick)
	{
		if (g_pInput->IsMousePush(0))
		{
			item->Shot(position, matR);
		}
	}
	
	//�A�C�e���̃A�b�v�f�[�g
	item->Update();
	//--------------------------------------------------------------//


	//-----------------�J�����֘A---------------------------------//
	

	D3DXVec3TransformCoord(&camVec, &camVec, &matR);	//�s��ƃx�N�g���̊|���Z
	D3DXVECTOR3 v(0, 0, 5);
	D3DXVec3TransformCoord(&v, &v, &matR);

	if (isRightCrick)
	{
		g_pCamera->SetTarget(position + v);				//�J�����̃^�[�Q�b�g���v���C���[�ɐݒ�
		g_pCamera->SetPos(position);					//�J��������l�̎��_��
	}
	else
	{
		g_pCamera->SetTarget(position);					//�J�����̃^�[�Q�b�g���v���C���[�ɐݒ�
		g_pCamera->SetPos(position + camVec);			//�J������position�ɍ쐬�����ړ��s����v���X����
	}

	
	//�}�E�X�J�[�\���𒆐S��
	g_pInput->SetCursorCenter();

	//���̈ʒu���L��
	GetCursorPos(&prevMousePos);

	//------------------------------------------------------------//

	//������Ԃ�
	return S_OK;
}
//player��Hit�֐�
//����:UnitBase�^�̃|�C���^
//(UnitBase�N���X���p�������q�N���X�̏�񂪈�����)
//�߂�l:HRESULT�^-�������ǂ���
HRESULT Player::Hit(UnitBase* pTarget)
{
	//�X�e�[�W�Ƃ̓����蔻��
	if (typeid(*pTarget) == typeid(Stage))
	{
		//pTarget���X�e�[�W�Ɣ��f���Astage�^�ɃL���X�g
		Stage* stage = (Stage*)pTarget;

		//RayCastData�^�̕ϐ������
		RayCastData ray;
		//���C���o��ʒu
		ray.start = position;
		ray.start.y = position.y+10;

		//���C�����������
		ray.dir = D3DXVECTOR3(0, -1, 0);
		
		//���C�̏����i�[�����A�h���X��n��
		stage->GetModel()->RayCast(&ray);

		//Ray���������Ă��邩��
		//�X�e�[�W�̈ʒuY���v���C���[�ʒuY-1���傫��������
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

		//�ڂ̑O�ɃX�e�[�W�����邩�̔���
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
		//�v���C���[�ƃJ�����̊ԂɃX�e�[�W�����邩
		D3DXVECTOR3 pToCdist = camVec-position;
		if (D3DXVec3Length(&v) < D3DXVec3Length(&pToCdist))
		{
			camVec = D3DXVECTOR3(position.x, 5, position.z);
		}

	}
	//�G�Ƃ̓����蔻��
	if (typeid(*pTarget) == typeid(Enemy))
	{
		static int count = 0;
		count--;
		Enemy* enemy = (Enemy*)pTarget;				//UnitBase�^��Enemy�^�ɃL���X�g
		BOOL flg = item->Hit(enemy->GetPos());		//�A�C�e���������������ǂ���
		if (flg)
		{
			enemy->KnockBack();
			count = 100;
		}
		D3DXVECTOR3 v = enemy->GetPos() - position; //�v���C���[����G�Ɍ������x�N�g�������߂�
		if (D3DXVec3Length(&v) < 2)					//�x�N�g���̒�����2�ȉ���������
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



