#pragma once

#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>

const char CLASS_NAME[] = "WinMain";
const char GAME_TITLE[] = "DirectX Fullscreen";

const bool FULLSCREEN = false;					// windowed or fullscreen

const UINT  GAME_WIDTH = 640;					// width of game in pixels
const UINT  GAME_HEIGHT = 480;					// height of game in pixels

// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;						// 타겟 프레임 레이트 (frame per sec)
const float MIN_FRAME_RATE = 10.0f;						// 최소 프레임 레이트
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			// 1 프레임당 최소 예상 시간
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;		// maximum time used in calculations

const UCHAR ESC_KEY = VK_ESCAPE;		// escape key
const UCHAR ALT_KEY = VK_MENU;			// Alt key
const UCHAR ENTER_KEY = VK_RETURN;		// Enter key