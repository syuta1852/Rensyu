/*===========================================================================================
�T�@���F�e�Q�[�����j�b�g�̐e�N���X
�쐬���F
�X�V���F
����ҁF
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include <typeinfo.h>


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�e�Q�[�����j�b�g�̐e�N���X
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class UnitBase
{
protected:
	//���j�b�g�̈ʒu
	D3DXVECTOR3	position;

public:
	virtual ~UnitBase() { }

	//�ǂݍ��ݏ���
	virtual HRESULT Load();

	//�X�V����
	virtual HRESULT Update();

	//�Փ˔��菈��
	//�����FpTarget	���肷�鑊�胆�j�b�g
	virtual HRESULT Hit(UnitBase* pTarget);

	//�`�揈��
	//�����Fhdc	�f�o�C�X�R���e�L�X�g�n���h��
	virtual HRESULT Render();

	//���݂̈ʒu���擾
	//�ߒl�F���݂̈ʒu
	D3DXVECTOR3 GetPos()	{ return position; }
};

