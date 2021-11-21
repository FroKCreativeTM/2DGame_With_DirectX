#pragma once

#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>

const char CLASS_NAME[] = "WinMain";
const char GAME_TITLE[] = "DirectX Fullscreen";

// �̹��� 
const char MENU_IMAGE[] = "pictures\\menu.png";
const char NEBULA_IMAGE[] = "pictures\\orion.jpg";
const char GAME_IMAGE[] = "pictures\\textures.png";

// �����
const char WAVE_BANK[] = "Audio\\data\\Win\\waveBank.xwb";
const char SOUND_BANK[] = "Audio\\data\\Win\\soundBank.xsb";
const char XGS_FILE[] = "Audio\\data\\Win\\SpaceWar.xgs";

// ���� ť
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
const float FRAME_RATE = 240.0f;						// Ÿ�� ������ ����Ʈ (frame per sec)
const float MIN_FRAME_RATE = 10.0f;						// �ּ� ������ ����Ʈ
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			// 1 �����Ӵ� �ּ� ���� �ð�
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;		// maximum time used in calculations


// �̸� ���ǵ� Ű
const UCHAR CONSOLE_KEY = '`';         // ` key
const UCHAR ESC_KEY = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY = VK_MENU;     // Alt key
const UCHAR ENTER_KEY = VK_RETURN;   // Enter key
const UCHAR SHIP1_LEFT_KEY = 'A';
const UCHAR SHIP1_RIGHT_KEY = 'D';
const UCHAR SHIP1_FORWARD_KEY = 'W';
const UCHAR SHIP1_FIRE_KEY = 'S';
const UCHAR SHIP2_LEFT_KEY = VK_LEFT; // left arrow
const UCHAR SHIP2_RIGHT_KEY = VK_RIGHT; // right arrow
const UCHAR SHIP2_FORWARD_KEY = VK_UP;   // up arrow
const UCHAR SHIP2_FIRE_KEY = VK_DOWN; // down arrow

// ���� Ÿ��
enum class WEAPON { TORPEDO, SHIP, PLANET };

const int FULL_HEALTH = 100;