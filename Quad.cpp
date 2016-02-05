#include "Quad.h"


Quad::Quad()
{
	pTexture = NULL;
}


Quad::~Quad()
{
	SAFE_RELEASE(pTexture);
}

HRESULT Quad::Load(char* fileName)
{
	//�u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture)))
	{
		MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", fileName, MB_OK);
		return E_FAIL;
	}
}

HRESULT Quad::Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex)
{
	struct Vertex
	{
		D3DXVECTOR3	pos;
		DWORD		color;
		D3DXVECTOR2	uv;
	};

	Vertex point[] =
	{
		D3DXVECTOR3(-0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0,0),      //����
		D3DXVECTOR3(0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1,0),       //�E��
		D3DXVECTOR3(-0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 1),    //����
		D3DXVECTOR3(0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 1)      //�E��
	};

	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)))
	{
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "���[���h�s��̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTexture(0, pTexture)))
	{
		MessageBox(0, "�e�N�X�`���̃Z�b�g�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}

	if (matTex != NULL)
	{
		if (FAILED(g_pDevice->SetTransform(D3DTS_TEXTURE0, matTex)))
		{
			MessageBox(0, "�e�N�X�`���s��̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
			return E_FAIL;
		}
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(Vertex))))
	{
		MessageBox(0, "�|���S���̕`��Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}
