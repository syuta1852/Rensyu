#pragma once
#include "Global.h"
#include <fbxsdk.h>

#pragma comment(lib,"libfbxsdk-mt.lib")

//���C�L���X�g�p�\����
struct RayCastData
{
	D3DXVECTOR3 start;	//���C���ˈʒu
	D3DXVECTOR3 dir;	//���C�̌����x�N�g��
	float		dist;	//�Փ˓_�܂ł̋���
	D3DXVECTOR3 normal;	//�Փ˓_�̖@��
	D3DXVECTOR3 pos;	//�Փˈʒu
	BOOL		hit;	//���C������������
};

class Fbx
{
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2  uv;
	};

	FbxManager*  manager;
	FbxImporter* importer;
	FbxScene*    scene;

	int			vertexCount;
	int			polygonCount;
	int			indexCount;
	int			materialCount;
	int*		polygonCountOfMaterial;

	D3DXMATRIX lastWorldMatrix; //�g������Ŋg���ړ������Ƃ��̂���

	IDirect3DVertexBuffer9   *pVertexBuffer;
	IDirect3DIndexBuffer9    **pIndexBuffer;
	D3DMATERIAL9			*pMaterial;
	LPDIRECT3DTEXTURE9       *pTexture;

	HRESULT      CheckNode(FbxNode* pNode);
	HRESULT      CheckMesh(FbxMesh* pMesh);

public:
	Fbx();
	~Fbx();
	HRESULT Load(char* fileName);
	HRESULT Draw(D3DXMATRIX* matWorld);
	BOOL RayCast(RayCastData *data);
	void MatrixIn(D3DXMATRIX* matW){ lastWorldMatrix = *matW; };
};

