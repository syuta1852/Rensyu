#include "Item.h"

//�R���X�g���N�^
Item::Item()
{
	position = D3DXVECTOR3(-999, -999, -999);	//�ʒu�̏�����
	move = D3DXVECTOR3(0, 0, 5);		//�ړ���
	model = NULL;						//���f���f�[�^�̃|�C���^�̏�����
	isShot = FALSE;						//����(����)���Ă��邩
}

//�f�X�g���N�^
Item::~Item()
{
	SAFE_DELETE(model);	//���f���f�[�^�̉��
}
HRESULT Item::Load()
{
	model = new Fbx;							//�A�C�e���̃��f���f�[�^�̍쐬
	model->Load("Assets\\models\\player.fbx");	//FBX�̃��[�h���Ă�("�����_�ł�enemy�ő�p")
	return S_OK;
}
//�`��
//����1:�v���C���[�̈ʒu
HRESULT Item::Render()
{
	if (isShot)//�^:���� �U:���݂��Ă��Ȃ�
	{
		D3DXMATRIX matW;	// �s��̍쐬
		D3DXMatrixTranslation(&matW, position.x, position.y, position.z); // �e���o��Ƃ��̃v���C���[�̈ʒu���s���
		model->Draw(&matW); //������s�����ɕ`��
	}
	return S_OK;
}

//�X�V
//�����Ȃ�
HRESULT Item::Update()
{
	static int count = 0;
	if (isShot)//���ˌ�Ȃ�
	{
		position += move;	//�ړ��ʕ��ړ�
		count++;			//�J�E���g�A�b�v
		if (count > 100)	//��苗���i�񂾂�
		{
			count = 0;		//�J�E���g��0��
			isShot = FALSE;	//�����ˏ�Ԃ�
			position = D3DXVECTOR3(-999, -999, -999);
			move = D3DXVECTOR3(0, 0, 5);
		}
	}
	return S_OK;
}
//�쐬���ꂽ��t���O�𗧂Ă�̂ƃv���C���[�̃|�W�V�����̊i�[
BOOL Item::Shot(D3DXVECTOR3 pos, D3DXMATRIX matW)
{
	if (!isShot)
	{
		position = pos;	//�|�W�V�����̊i�[
		D3DXVec3TransformCoord(&move, &move, &matW);
	}
	isShot = TRUE;	//���ˏ�Ԃ�
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