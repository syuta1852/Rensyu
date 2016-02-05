#include "Line.h"


HRESULT Line::Draw(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	D3DXVECTOR3 point[2];
	point[0] = start;
	point[1] = end;

	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ)))
	{
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	D3DXMATRIX mReset;
	D3DXMatrixIdentity(&mReset);
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, &mReset)))
	{
		MessageBox(0, "���[���h�s��̃��Z�b�g�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}


	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, point, sizeof(D3DXVECTOR3))))
	{
		MessageBox(0, "���C���̕`��Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


HRESULT Line::Draw(D3DXVECTOR3 start, D3DXVECTOR3 end, DWORD color)
{
	struct Vertex
	{
		D3DXVECTOR3 pos;
		DWORD color;
	};

	Vertex point[2];
	point[0].pos =	 start;
	point[0].color = color;
	point[1].pos =	 end;
	point[1].color = color;

	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE)))
	{
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	D3DXMATRIX mReset;
	D3DXMatrixIdentity(&mReset);
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, &mReset)))
	{
		MessageBox(0, "���[���h�s��̃��Z�b�g�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		MessageBox(0, "�e�N�X�`���̃��Z�b�g�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->DrawPrimitiveUP(
		    D3DPT_LINELIST, 1, point, sizeof(Vertex))))
	{
		MessageBox(0, "���C���̕`��Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}