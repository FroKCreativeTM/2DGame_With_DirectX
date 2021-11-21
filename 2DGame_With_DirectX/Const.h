#pragma once

#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>

const char CLASS_NAME[] = "WinMain";
const char GAME_TITLE[] = "DirectX Fullscreen";

// 이미지 
const char NEBULA_IMAGE[] = "pictures\\orion.jpg";  
const char GAME_IMAGE[] = "pictures\\textures.png";

// 오디오
const char WAVE_BANK[] = "Audio\\data\\Win\\waveBank.xwb";
const char SOUND_BANK[] = "Audio\\data\\Win\\soundBank.xsb";
const char XGS_FILE[] = "Audio\\data\\Win\\SpaceWar.xgs";

// 사운드 큐
const char CHEER[] = "cheer";
const char COLLIDE[] = "collide";
const char EXPLODE[] = "explode";
const char ENGINE1[] = "engine1";
const char ENGINE2[] = "engine2";
const char TORPEDO_CRASH[] = "torpedoCrash";
const char TORPEDO_FIRE[] = "torpedoFire";
const char TORPEDO_HIT[] = "torpedoHit";
const char ACTION_THEME[] = "actionTheme";


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
const UCHAR CONSOLE_KEY = VK_OEM_3;		// ~키

// 플레이어 관련 변수
const int  SHIP_START_FRAME = 0;            // 플레이어 쉽 애니메이션 시작 프레임
const int  SHIP_END_FRAME = 3;              // 플레이어 쉽 애니메이션 끝 프레임
const float SHIP_ANIMATION_DELAY = 0.2f;    // 프레임 간 시간
const int  SHIP_COLS = 2;                   // 쉽 사진의 column 수
const int  SHIP_WIDTH = 32;                 // 쉽 이미지의 폭
const int  SHIP_HEIGHT = 32;                // 쉽 이미지의 높이
const float ROTATION_RATE = 180.0f;         // 1초당 degree
const float SCALE_RATE = 0.2f;              // 1초당 스케일 변화량
const float SHIP_SPEED = 100.0f;            // 1초당 스케일
const float SHIP_SCALE = 1.5f;              // 쉽 스케일

const UCHAR SHIP_LEFT_KEY = VK_LEFT;     // left arrow
const UCHAR SHIP_RIGHT_KEY = VK_RIGHT;    // right arrow
const UCHAR SHIP_UP_KEY = VK_UP;       // up arrow
const UCHAR SHIP_DOWN_KEY = VK_DOWN;     // down arrow

// 무기 타입
enum WEAPON { TORPEDO, SHIP, PLANET };