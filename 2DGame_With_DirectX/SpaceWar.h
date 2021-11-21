#pragma once

#include <string>
#include "CGameEngine.h"
#include "Graphics/CTextureManager.h"
#include "Graphics/CImage.h"

#include "Scene/CDashboard.h"

#include "Entity/CShip.h"
#include "Entity/CPlanet.h"
#include "Entity/CTorpedo.h"

namespace NSSpaceWar {
	const char FONT[] = "Arial Bold";
	const int FONT_BIG_SIZE = 256;
	const int FONT_SCORE_SIZE = 48;
	const COLOR_ARGB FONT_COLOR = NSGraphics::YELLOW;
	const COLOR_ARGB SHIP1_COLOR = NSGraphics::BLUE;
	const COLOR_ARGB SHIP2_COLOR = NSGraphics::YELLOW;
	const int SCORE_Y = 10;
	const int SCORE1_X = 60;
	const int SCORE2_X = GAME_WIDTH - 80;
	const int HEALTHBAR_Y = 30;
	const int SHIP1_HEALTHBAR_X = 40;
	const int SHIP2_HEALTHBAR_X = GAME_WIDTH - 100;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 5;	
	const int BUF_SIZE = 20;	
	const int ROUND_TIME = 5;	// 새 라운드를 시작하기 까지 걸리는 시간
}

class SpaceWar : public CGameEngine {
private : 
	// 필요한 변수들
	CTextureManager		m_menuTexture;
	CTextureManager		m_nebulaTexture;
	CTextureManager		m_gameTexture;
	CShip				m_ship1, m_ship2;
	CTorpedo			m_torpedo1, m_torpedo2;
	CPlanet				m_planet;
	CImage				m_nebula;
	CImage				m_menu;
	CDashboard			m_healthBar;
	CTextDX				m_fontBig;
	CTextDX				m_fontScore;
	bool				m_isMenuOn;
	bool				m_isCountdownOn;
	float				m_countDownTimer;
	char				m_buffer[NSSpaceWar::BUF_SIZE];
	bool				m_ship1Scored, m_ship2Scored;
	bool				m_isRoundOver;
	float				m_roundTimer;
	int					m_ship1Score, m_ship2Score;
	bool				m_isMusicOff;

public : 
	// 생성자 및 소멸자
	SpaceWar();
	virtual ~SpaceWar();

	// 게임 초기화
	void Initialize(HWND hwnd);
	void Update();
	void Ai();
	void Collisions();
	void Render();

	void ConsoleCommand();
	void RoundStart();

	void ReleaseAll();
	void ResetAll();
};

