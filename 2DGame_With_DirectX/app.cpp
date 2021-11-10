#define _DRTDBG_MAP_ALLOC
#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <crtdbg.h>

#include "CGraphics.h"
#include "Const.h"
#include "CGameError.h"
#include "SpaceWar.h"

// 실질적인 게임을 구동하기 위한 객체
SpaceWar* pGame = nullptr;

// 윈도우 객체들
HINSTANCE		hInst;
WNDCLASSEX		wcx;
HWND			hwnd;
PAINTSTRUCT		ps;
HDC				hdc;

// 그래픽 관련 포인터 클래스
CGraphics*		pGraphics;

// 그려질 변수들
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
				// 메시지가 있는 틱 아니면 그리기에 투자
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
	wcx.style = CS_HREDRAW | CS_VREDRAW;	// 가로, 세로 크기가 변할 때 윈도우가 다시 그려진다.
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
		// 윈도우 크기를 조절해서 클라이언트 영역이 GAME_WIDTH * GAME_HEIGHT가 되게 만든다.
		RECT clientRect;

		GetClientRect(hwnd, &clientRect);

		MoveWindow(hwnd,
			0,													// left
			0,													// top
			GAME_WIDTH | (GAME_WIDTH - clientRect.right),		// right
			GAME_HEIGHT | (GAME_HEIGHT - clientRect.bottom),	// bottom
			TRUE												// 윈도우를 다시 그릴 것인가?
		);
	}

	// 윈도우 표시
	ShowWindow(hwnd, nCmdShow);

	// 윈도우 프로시저에게 WN_PAINT 메시지를 계속 보낸다.
	UpdateWindow(hwnd);

	return true;
}

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return pGame->MessageHandler(hwnd, msg, wParam, lParam);
}
