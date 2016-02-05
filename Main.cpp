/*=======================================================================================================
�T�@���F�E�B���h�E�̊Ǘ��i�G���g���[�|�C���g�j
�쐬���F
�X�V���F
����ҁF
=======================================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Global.h"
#include <time.h>
#include "Game.h"

//---------------���������[�N���o---------------
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



//---------------�v���g�^�C�v�錾---------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);


//----------------�O���[�o���ϐ�----------------
TCHAR	szClassName[] = "OriginalGame"; //�E�B���h�E�N���X��
Game	*game;							//�Q�[���I�u�W�F�N�g
HWND	hWnd;							//�E�B���h�E�n���h��
BOOL isEnd = FALSE;


/////////////////////////////////////////////////////////////////////////////////////////////////////

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//�G���g���[�|�C���g
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
#if _DEBUG
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


	MSG msg;

	//�E�B���h�E�N���X�̓o�^
	if (!InitApp(hCurInst))
		return FALSE;

	//�E�B���h�E����
	if (!InitInstance(hCurInst, nCmdShow))
		return FALSE;

	//�����_���̏���
	srand((unsigned)time(NULL));

	//�Q�[���I�u�W�F�N�g���쐬
	game = new Game;

	//Direct3D�̏�����
	if (FAILED(game->InitD3d(hWnd)))
	{
		return FALSE;
	}

	//���͏����̏�����
	g_pInput = new Input();
	g_pInput->Init(hWnd);

	//�ǂݍ��ݏ���
	if (FAILED(game->Load()))
	{
		return FALSE;
	}

	// ���b�Z�[�W���擾
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (isEnd)
			{
				//�����I��
				break;
			}


			//�Q�[���̍X�V
			if (FAILED(game->Update()))
			{
				return FALSE;
			}

			//�Փ˔���
			if (FAILED(game->Hit()))
			{
				return FALSE;
			}

			//�Q�[����ʂ̕`��
 			if (FAILED(game->Render()))
			{
				return FALSE;
			}
		}
	}

	return (int)msg.wParam;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �E�B���h�E�N���X�̓o�^
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);           // �\���̂̃T�C�Y
	wc.style = CS_HREDRAW | CS_VREDRAW;       // �N���X�̃X�^�C��
	wc.lpfnWndProc = WndProc;                 // �v���V�[�W����
	wc.cbClsExtra = 0;                        // �⏕������
	wc.cbWndExtra = 0;                        // �⏕������
	wc.hInstance = hInst;                     // �C���X�^���X
	wc.hIcon = (HICON)LoadImage(              // �A�C�R��
		NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = (HCURSOR)LoadImage(          // �J�[�\��
		NULL, MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground =                        // �w�i�u���V
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;                   // ���j���[��
	wc.lpszClassName = szClassName;           // �N���X��
	wc.hIconSm = (HICON)LoadImage(            // �������A�C�R��
		NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	return (RegisterClassEx(&wc));
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �E�B���h�E�̐���
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{


	//�N���C�A���g�̈�T�C�Y����A�E�B���h�E�T�C�Y���v�Z
	RECT r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�̍쐬
	hWnd = CreateWindow(
		szClassName,			// �N���X��
		szClassName,			// �E�B���h�E��
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,			// x�ʒu
		CW_USEDEFAULT,			// y�ʒu
		r.right - r.left,		// �E�B���h�E��
		r.bottom - r.top,		// �E�B���h�E����
		NULL,					// �e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
		NULL,					// ���j���[�n���h���A�N���X���j���[���g���Ƃ���NULL
		hInst,					// �C���X�^���X�n���h��
		NULL					// �E�B���h�E�쐬�f�[�^
		);

	//�E�B���h�E�̍쐬�Ɏ��s�����ꍇ
	if (!hWnd)
	{
		return FALSE;
	}

	// �E�B���h�E�̕\����Ԃ�ݒ�
	ShowWindow(hWnd, nCmdShow);

	// �E�B���h�E���X�V
	UpdateWindow(hWnd);

	return TRUE;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �E�B���h�E�v���V�[�W��
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	POINT mousePos;
	switch (msg)
	{

	case WM_MOUSEMOVE:
		mousePos.x = LOWORD(lp);
		mousePos.y = HIWORD(lp);
		g_pInput->SetMousePos(mousePos);
		break;
	//���E�B���h�E�������ꂽ
	case WM_DESTROY:
		isEnd = TRUE;

		//���͏����J��
		SAFE_DELETE(g_pInput);

		//�Q�[���I�u�W�F�N�g�̊J��
		SAFE_DELETE(game);

		//�v���O�����I��
		PostQuitMessage(0);
		break;
	
	//�����̑�
	default:
		return (DefWindowProc(hWnd, msg, wp, lp));
	}
	return 0;
}
