#include "UnitBase.h"	//UnitBase�N���X�w�b�_�[
#include "Fbx.h"		//Fbx�N���X�w�b�_�[

//UnitBase�N���X���p������Enemy�N���X�̍쐬
class Enemy :public UnitBase
{
	Fbx*		enemy;		//Fbx�N���X�̃|�C���^
	D3DXVECTOR3 move;		//�ړ��p�̃x�N�g��
	float		dir;		//��]�Ǝ��_�̊p�x
	D3DXVECTOR3 WallNormal;	//�ǂ̖@�����i�[���邽�߂̕ϐ�
	BOOL		isWallHit;	//�ǂƓ������Ă��邩�̔���p
	D3DXVECTOR3 targetPos;	//�������Ƃ��Ƀv���C���[�̈ʒu���i�[���邽�߂̕ϐ�
	float		wallDist;	//�v���C���[�̈ʒu�ƕǂ̈ʒu�̂ǂ��炪�߂����̔���p
	DWORD		state;		//���݂̏�Ԃ��i�[���邽�߂̕ϐ�
	DWORD		timer;
	int			count;
	float		y_prev;		//Y���̌��̍��W�l�����
	BOOL		jump_flg;	//�W�����v���Ă��邩
	BOOL		isInvisible;//�v���C���[�������Ȃ��ꏊ�ɂ��邩

	//���
	enum
	{
		STATE_ROTA,		//��]
		STATE_WALK,		//�O�i���
		STATE_HIT,		//�e�ɓ��������Ƃ��̏���
		STATE_DEAD,		//�|���ꂽ�Ƃ��̏���
		STATE_RUN,		//�ǐ�
	};


public:
	Enemy();									//�R���X�g���N�^
	~Enemy();									//�f�X�g���N�^
	HRESULT Load();								//Load�֐�(�I�[�o�[���C�h)
	HRESULT Render();							//Render�֐�(�I�[�o�[���C�h)
	HRESULT Update();							//Update�֐�(�I�[�o�[���C�h)
	HRESULT Hit(UnitBase* pTarget);				//Hit�֐�(�I�[�o�[���C�h)
	BOOL	Search();							//�G�������Ă��邩�̔���֐�
	D3DXVECTOR3 GetPos(){ return position; }	//�|�W�V�����̃A�N�Z�X�֐�

	void KnockBack();	//�e������������ԂɕύX����֐�
	void Kill();		//�e������������ԂŃv���C���[�ɐڐG�����Ƃ��̏����֐�
};
