#define _DRTDBG_MAP_ALLOC
#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <crtdbg.h>

#include "CGraphics.h"
#include "Const.h"
#include "CGameError.h"
#include "SpaceWar.h"

// �������� ������ �����ϱ� ���� ��ü
SpaceWar* pGame = nullptr;

// ������ ��ü��
HINSTANCE		hInst;
WNDCLASSEX		wcx;
HWND			hwnd;
PAINTSTRUCT		ps;
HDC				hdc;

// �׷��� ���� ������ Ŭ����
CGraphics*		pGraphics;

// �׷��� ������
RECT  rect;
TCHAR ch = ' ';

bool InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MSG msg;
	
	pGame = new SpaceWar;

	if (!InitWindow(hInstance, nCmdShow)) {
		return false;
	}

	try {
		pGame->Initialize(hwnd);

		int done = 0;

		while (!done) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					done = 1;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				// �޽����� �ִ� ƽ �ƴϸ� �׸��⿡ ����
				pGame->Run(hwnd);
			}
		}
		SAFE_DELETE(pGame);
		return msg.wParam;
	}
	catch (const CGameError& err)
	{
		pGame->DeleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch (...)
	{
		pGame->DeleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}

	SAFE_DELETE(pGame);
	return 0;	
}

bool InitWindow(HINSTANCE hInstance, int nCmdShow) {

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;	// ����, ���� ũ�Ⱑ ���� �� �����찡 �ٽ� �׷�����.
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = nullptr;
	wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = nullptr;

	if (RegisterClassEx(&wcx) == 0) {
		return false;
	}

	DWORD style;

	if (FULLSCREEN) {
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else {
		style = WS_OVERLAPPEDWINDOW;
	}

	hwnd = CreateWindow(
		CLASS_NAME,
		GAME_TITLE,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		GAME_WIDTH, GAME_HEIGHT,
		(HWND) nullptr, (HMENU) nullptr,
		hInstance,
		(LPVOID)nullptr
	);

	if (!hwnd) {
		return false;
	}

	if (!FULLSCREEN) {
		// ������ ũ�⸦ �����ؼ� Ŭ���̾�Ʈ ������ GAME_WIDTH * GAME_HEIGHT�� �ǰ� �����.
		RECT clientRect;

		GetClientRect(hwnd, &clientRect);

		MoveWindow(hwnd,
			0,													// left
			0,													// top
			GAME_WIDTH | (GAME_WIDTH - clientRect.right),		// right
			GAME_HEIGHT | (GAME_HEIGHT - clientRect.bottom),	// bottom
			TRUE												// �����츦 �ٽ� �׸� ���ΰ�?
		);
	}

	// ������ ǥ��
	ShowWindow(hwnd, nCmdShow);

	// ������ ���ν������� WN_PAINT �޽����� ��� ������.
	UpdateWindow(hwnd);

	return true;
}

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return pGame->MessageHandler(hwnd, msg, wParam, lParam);
}
