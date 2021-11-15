#pragma once

#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>

const char CLASS_NAME[] = "WinMain";
const char GAME_TITLE[] = "DirectX Fullscreen";

// �̹��� 
const char NEBULA_IMAGE[] = "pictures\\orion.jpg";  
const char GAME_IMAGE[] = "pictures\\textures.png";

// �����
const char WAVE_BANK[] = "Audio\\data\\Win\\WavesExample1.xwb";
const char SOUND_BANK[] = "Audio\\data\\Win\\SoundsExample1.xsb";

// ���� ť
const char BEEP1[] = "beep1";
const char BEEP2[] = "beep2";
const char BEEP3[] = "beep3";
const char BEEP4[] = "beep4";
const char HIT[] = "hit";


const bool FULLSCREEN = false;					// windowed or fullscreen

const UINT  GAME_WIDTH = 640;					// width of game in pixels
const UINT  GAME_HEIGHT = 480;					// height of game in pixels

// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;						// Ÿ�� ������ ����Ʈ (frame per sec)
const float MIN_FRAME_RATE = 10.0f;						// �ּ� ������ ����Ʈ
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			// 1 �����Ӵ� �ּ� ���� �ð�
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;		// maximum time used in calculations

const UCHAR ESC_KEY = VK_ESCAPE;		// escape key
const UCHAR ALT_KEY = VK_MENU;			// Alt key
const UCHAR ENTER_KEY = VK_RETURN;		// Enter key

// �÷��̾� ���� ����
const int  SHIP_START_FRAME = 0;            // �÷��̾� �� �ִϸ��̼� ���� ������
const int  SHIP_END_FRAME = 3;              // �÷��̾� �� �ִϸ��̼� �� ������
const float SHIP_ANIMATION_DELAY = 0.2f;    // ������ �� �ð�
const int  SHIP_COLS = 2;                   // �� ������ column ��
const int  SHIP_WIDTH = 32;                 // �� �̹����� ��
const int  SHIP_HEIGHT = 32;                // �� �̹����� ����
const float ROTATION_RATE = 180.0f;         // 1�ʴ� degree
const float SCALE_RATE = 0.2f;              // 1�ʴ� ������ ��ȭ��
const float SHIP_SPEED = 100.0f;            // 1�ʴ� ������
const float SHIP_SCALE = 1.5f;              // �� ������

const UCHAR SHIP_LEFT_KEY = VK_LEFT;     // left arrow
const UCHAR SHIP_RIGHT_KEY = VK_RIGHT;    // right arrow
const UCHAR SHIP_UP_KEY = VK_UP;       // up arrow
const UCHAR SHIP_DOWN_KEY = VK_DOWN;     // down arrow

// ���� Ÿ��
enum WEAPON { TORPEDO, SHIP, PLANET };