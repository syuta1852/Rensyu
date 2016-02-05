#include "Camera.h"


Camera::Camera()
{
	pos =	D3DXVECTOR3(0, 8, -5);
	target =D3DXVECTOR3(0, 0, 10);
	upVec =	D3DXVECTOR3(0, 1, 0);

	angle = 60;
	aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
	nearClip = 0.1f;
	farClip = 1000.0f;
}


Camera::~Camera()
{
}

HRESULT Camera::SetView()
{
	D3DXMatrixLookAtLH(&view, &pos, &target, &upVec);

	if (FAILED(g_pDevice->SetTransform(D3DTS_VIEW, &view)))
	{
		MessageBox(0, "ビュー行列の設定に失敗しました", "Camera", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Camera::SetProjection()
{
	D3DXMatrixPerspectiveFovLH(&proj, 
		D3DXToRadian(angle), aspect, 
		nearClip, farClip);

	if (FAILED(g_pDevice->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		MessageBox(0, "射影行列の設定に失敗しました", "Camera", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Camera::Update()
{
	if (FAILED(SetView()))
	{
		return E_FAIL;
	}

	if (FAILED(SetProjection()))
	{
		return E_FAIL;
	}

	return S_OK;
}