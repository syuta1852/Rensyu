//�쐬��:���X�ؗT��
//�X�V��:11/2
#include "Enemy.h"
#include "Player.h"
#include "Stage.h"

//�R���X�g���N�^
Enemy::Enemy()
{
	position = D3DXVECTOR3(0, 40, 0);			//�ʒu�̏�����
	enemy = NULL;								//enemy�̃|�C���^������
	move = D3DXVECTOR3(0, 0, (rand()%10));				//�ړ��x�N�g���̏�����
	dir = 0;									//��]�p�ϐ��̏�����
	targetPos = D3DXVECTOR3(-999, -999, -999);	//�v���C���[�ʒu�̏�����(���肦�Ȃ����l��)
	timer = 0;
	count = 1;
	y_prev = 0;
	jump_flg = FALSE;
}
//�f�X�g���N�^
Enemy::~Enemy()
{
	SAFE_DELETE(enemy);	//enemy��DELETE
}

//Load�֐�
//����:�Ȃ�
//�ߒl:�������ǂ���
HRESULT Enemy::Load()
{
	enemy = new Fbx;							//Fbx�N���X�̃C���X�^���X�𐶐�
	enemy->Load("Assets\\Models\\enemy.fbx");	//Fbx�f�[�^��Load���Ăяo��
	return S_OK;
}
//Render�֐�
//����:�Ȃ�
//�ߒl:�������ǂ���
HRESULT Enemy::Render()
{
	D3DXMATRIX matW,matR;	//�`��ɕK�v�ȍs��̍쐬
	D3DXMatrixTranslation(&matW, position.x, position.y, position.z);	//�`��ʒu��position��
	D3DXMatrixRotationY(&matR,D3DXToRadian(dir));
	matW = matR * matW;
	/*static int count;*/
	if (state == STATE_HIT)
	{
		count++;
		if (count%10 == 0)
		{
			enemy->Draw(&matW);	//�`��
			count = 0;
			
		}
	}
	else if (state == STATE_DEAD)
	{

	}
	else
	{
		enemy->Draw(&matW);	//�`��
		count = 0;
	}
	return S_OK;
}
//Update�֐�
//����:�Ȃ�
//�ߒl:�������ǂ���
HRESULT Enemy::Update()
{
	//�^�C�}�[�̐錾
	//��Ƀ^�C�}�[���J�E���g(���̍s����������0�ɂȂ�)
	//state�̏�Ԃ��ς�邽�тɃ��Z�b�g�����
	timer++;

	static int count = 0;

	//-----�x�N�g���̉�]-----
	move = D3DXVECTOR3(0, move.y, 0.05);				//�x�N�g���̏�����
	D3DXMATRIX matR;								//��]�s��쐬�p
	D3DXMatrixRotationY(&matR, D3DXToRadian(dir));	//��]�s��̒��g���쐬
	D3DXVec3TransformCoord(&move, &move, &matR);	//�x�N�g���ƍs��̊|���Z

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

		//�O�i
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
		//��莞�ԓ��������]
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
		
		  //�ǐՒ�
		  position += move * 2;

		  D3DXVECTOR3 pToDist = targetPos - position;
		  D3DXVECTOR3 cross;//�O�ϊi�[�p
		  D3DXVec3Cross(&cross, &move, &pToDist);

		  //���E�ǂ���ɂ��邩
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
	//�X�e�[�W�Ƃ̓����蔻��
	if (typeid(*pTarget) == typeid(Stage))
	{
		Stage* stage = (Stage*)pTarget;

		RayCastData ray;
		//���C���o��ʒu
		ray.start = position;
		ray.start.y = position.y +10;

		//���C�����������
		ray.dir = D3DXVECTOR3(0, -1, 0);

		//���C�̏����i�[�����A�h���X��n��
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
		//�v���C���[�̈ʒu��ۑ�
		targetPos = player->GetPos();
		isInvisible = player->GetInvisible();
	}
	return S_OK;
}

BOOL Enemy::Search()
{
	//�v���C���[�̈ʒu-�����̈ʒu�Ńv���C���[�Ɍ����������̂킩��Ȃ��x�N�g�������߂�
	D3DXVECTOR3 pToDist = targetPos - position;	
	float pDist = D3DXVec3Length(&pToDist);	//�x�N�g���̒����𑪂�ϐ��Ɋi�[

	//���ړ����Ă�������ւ̃x�N�g����
	//�v���C���[�̈ʒu�ւ̃x�N�g���𐳋K��(���ςŊp�x�����߂邽��)
	D3DXVec3Normalize(&move,&move);			//���ړ����Ă���x�N�g��
	D3DXVec3Normalize(&pToDist, &pToDist);	//�v���C���[�ʒu�ւ̃x�N�g��

	//���ςŊp�x�����߂�(Dot�֐��̖ߒl��COS�̂���acos�֐��œx�ɒ����K�v����)
	float d = D3DXVec3Dot(&move, &pToDist);
	//�R�T�C���l��x���ɕϊ�
	d = acos(d);

	//�x�N�g���̒����Ɗp�x���g���Č����Ă��邩����
	if (pDist <= 20 && d <= D3DXToRadian(40))
	{
		return TRUE;
	}

	return FALSE;
}

//Enemy���q�b�g��Ԃ�
void Enemy::KnockBack()
{
	if (state != STATE_RUN)
	{
		state = STATE_HIT;
		timer = 0;
	}
}

//Enemy��|�����Ƃ��̏���
void Enemy::Kill()
{
	state = STATE_DEAD;
	position = D3DXVECTOR3(-999, -999, 0);
}