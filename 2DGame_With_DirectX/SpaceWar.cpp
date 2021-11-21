#include "SpaceWar.h"

SpaceWar::SpaceWar() : 
	m_isMenuOn(true),
	m_isCountdownOn(false),
	m_isRoundOver(false),
	m_ship1Score(0),
	m_ship2Score(0),
	m_ship1Scored(false),
	m_ship2Scored(false)
{
	m_initialized = false;
}

SpaceWar::~SpaceWar() {
	ReleaseAll();
}

void SpaceWar::Initialize(HWND hwnd) {
	CGameEngine::Initialize(hwnd);

	// DirectX 폰트를 초기화한다.
	m_fontBig.Initialize(m_pGraphics, NSSpaceWar::FONT_BIG_SIZE, false, false, NSSpaceWar::FONT);
	m_fontBig.SetFontColor(NSSpaceWar::FONT_COLOR);
	m_fontScore.Initialize(m_pGraphics, NSSpaceWar::FONT_SCORE_SIZE, false, false, NSSpaceWar::FONT);

	// 텍스처와 이미지를 설정한다.
	if (!m_menuTexture.Initialize(m_pGraphics, MENU_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"메뉴 텍스처 초기화 실패"));
	}

	if (!m_nebulaTexture.Initialize(m_pGraphics, NEBULA_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"네뷸라 텍스처 초기화 실패"));
	}

	if (!m_gameTexture.Initialize(m_pGraphics, GAME_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"게임 텍스처 초기화 실패"));
	}

	if (!m_menu.Initialize(m_pGraphics, 0, 0, 0, &m_menuTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"메뉴 이미지 초기화 실패"));
	}

	if (!m_nebula.Initialize(m_pGraphics, 0, 0, 0, &m_nebulaTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"네뷸라 이미지 초기화 실패"));
	}

	if (!m_planet.Initialize(this, NSPlanet::WIDTH, NSPlanet::HEIGHT, 
		NSPlanet::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"행성 이미지 초기화 실패"));
	}

	if (!m_ship1.Initialize(this, NSShip::WIDTH, NSShip::HEIGHT, 
		NSShip::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"쉽1 이미지 초기화 실패"));
	}

	m_ship1.SetFrames(NSShip::SHIP1_START_FRAME, NSShip::SHIP1_END_FRAME);
	m_ship1.SetCurrentFrame(NSShip::SHIP1_START_FRAME);
	m_ship1.SetColorFilter(SETCOLOR_ARGB(255, 230, 230, 255));
	m_ship1.SetMass(NSShip::MASS);

	if (!m_ship2.Initialize(this, NSShip::WIDTH, NSShip::HEIGHT, NSShip::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"쉽2 이미지 초기화 실패"));
	}

	m_ship2.SetFrames(NSShip::SHIP2_START_FRAME, NSShip::SHIP2_END_FRAME);
	m_ship2.SetCurrentFrame(NSShip::SHIP2_START_FRAME);
	m_ship2.SetColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));
	m_ship2.SetMass(NSShip::MASS);

	// 미사일 초기화
	if (!m_torpedo1.Initialize(this, NSTorpedo::WIDTH, NSTorpedo::HEIGHT,
		NSTorpedo::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"미사일1 이미지 초기화 실패"));
	}

	m_torpedo1.SetFrames(NSTorpedo::START_FRAME, NSTorpedo::END_FRAME);
	m_torpedo1.SetCurrentFrame(NSTorpedo::START_FRAME);
	m_torpedo1.SetColorFilter(SETCOLOR_ARGB(255, 128, 128, 255));

	if (!m_torpedo2.Initialize(this, NSTorpedo::WIDTH, NSTorpedo::HEIGHT,
		NSTorpedo::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"미사일2 이미지 초기화 실패"));
	}

	m_torpedo2.SetFrames(NSTorpedo::START_FRAME, NSTorpedo::END_FRAME);
	m_torpedo2.SetCurrentFrame(NSTorpedo::START_FRAME);
	m_torpedo2.SetColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));

	// 체력 표시줄
	m_healthBar.Initialize(m_pGraphics, &m_gameTexture, 0, NSSpaceWar::HEALTHBAR_Y,
		2.0f, NSGraphics::WHITE);

	// 우주선을 시계 방향 궤도로 움직이는 행성의 반대편에서 시작하게 만든다
	m_ship1.SetX(GAME_WIDTH / 4 - NSShip::WIDTH);
	m_ship1.SetY(GAME_HEIGHT / 2 - NSShip::HEIGHT);
	m_ship2.SetX(GAME_WIDTH - GAME_WIDTH / 4);
	m_ship2.SetY(GAME_HEIGHT / 2);
	m_ship1.SetVelocity(VECTOR2(0, -NSShip::SPEED));
	m_ship2.SetVelocity(VECTOR2(0, NSShip::SPEED));

	m_pAudio->PlayCue(ACTION_THEME);

	return;
}

void SpaceWar::Update() {
	if (m_isMenuOn) {
		if (m_pInput->AnyKeyPressed()) {
			m_isMenuOn = false;
			m_pInput->ClearAll();
			RoundStart();
		}
	}
	else if (m_isCountdownOn) {
		m_countDownTimer -= m_frameTime;
		if (m_countDownTimer <= 0) {
			m_isCountdownOn = false;
		}
	}
	else {
		if (m_ship1.GetActive()) {
			// 엔진이 켜진다면
			if (m_pInput->IsKeyDown(SHIP1_FORWARD_KEY) || m_pInput->GetGamepadDPadUp(0)) {
				m_ship1.SetEngineOn(true);
				m_pAudio->PlayCue(ENGINE1);
			}
			else {
				m_ship1.SetEngineOn(false);
				m_pAudio->StopCue(ENGINE1);
			}
			m_ship1.Rotate(NSShip::DIRECTION::NONE);

			// 왼쪽으로 방향을 돌린다면
			if (m_pInput->IsKeyDown(SHIP1_LEFT_KEY) || m_pInput->GetGamepadDPadLeft(0)) {
				m_ship1.Rotate(NSShip::DIRECTION::LEFT);
			}
			// 오른쪽으로 방향을 돌린다면
			if (m_pInput->IsKeyDown(SHIP1_RIGHT_KEY) || m_pInput->GetGamepadDPadRight(0)) {
				m_ship1.Rotate(NSShip::DIRECTION::RIGHT);
			}
			// 오른쪽으로 방향을 돌린다면
			if (m_pInput->IsKeyDown(SHIP1_FIRE_KEY) || m_pInput->GetGamepadA(0)) {
				m_torpedo1.Fire(&m_ship1);
			}
		}
		if (m_ship2.GetActive()) {
			// 엔진이 켜진다면
			if (m_pInput->IsKeyDown(SHIP2_FORWARD_KEY) || m_pInput->GetGamepadDPadUp(0)) {
				m_ship2.SetEngineOn(true);
				m_pAudio->PlayCue(ENGINE2);
			}
			else {
				m_ship2.SetEngineOn(false);
				m_pAudio->StopCue(ENGINE2);
			}
			m_ship2.Rotate(NSShip::DIRECTION::NONE);

			// 왼쪽으로 방향을 돌린다면
			if (m_pInput->IsKeyDown(SHIP2_LEFT_KEY) || m_pInput->GetGamepadDPadLeft(0)) {
				m_ship2.Rotate(NSShip::DIRECTION::LEFT);
			}
			// 오른쪽으로 방향을 돌린다면
			if (m_pInput->IsKeyDown(SHIP2_RIGHT_KEY) || m_pInput->GetGamepadDPadRight(0)) {
				m_ship2.Rotate(NSShip::DIRECTION::RIGHT);
			}
			// 오른쪽으로 방향을 돌린다면
			if (m_pInput->IsKeyDown(SHIP2_FIRE_KEY) || m_pInput->GetGamepadA(0)) {
				m_torpedo2.Fire(&m_ship2);
			}
		}
		if (m_isRoundOver) {
			m_roundTimer -= m_frameTime;
			if (m_roundTimer <= 0) {
				RoundStart();
			}
		}
	}

	m_ship1.GravityForce(&m_planet, m_frameTime);
	m_ship2.GravityForce(&m_planet, m_frameTime);
	m_torpedo1.GravityForce(&m_planet, m_frameTime);
	m_torpedo2.GravityForce(&m_planet, m_frameTime);

	// 모든 개체 갱신
	m_planet.Update(m_frameTime);
	m_ship1.Update(m_frameTime);
	m_ship2.Update(m_frameTime);
	m_torpedo1.Update(m_frameTime);
	m_torpedo2.Update(m_frameTime);
}

void SpaceWar::Ai() {
}

void SpaceWar::Collisions() {
	VECTOR2 collisionVector;

	// 우주선 1과 행성이 충돌 발생시
	if (m_ship1.CollidesWith(m_planet, collisionVector)) {
		// 행성에 부딪혀 튕겨져 나온다.
		m_ship1.ToOldPosition();
		m_ship1.Damage(WEAPON::PLANET);
		m_pInput->GamePadVibrateLeft(0, 65535, 1.0);
	}
	// 우주선 2과 행성이 충돌 발생시
	if (m_ship2.CollidesWith(m_planet, collisionVector)) {
		// 행성에 부딪혀 튕겨져 나온다.
		m_ship2.ToOldPosition();
		m_ship2.Damage(WEAPON::PLANET);
		m_pInput->GamePadVibrateLeft(1, 65535, 1.0);
	}
	// 우주선 1과 2 충돌 발생시
	if (m_ship1.CollidesWith(m_ship2, collisionVector)) {
		m_ship1.Bounce(collisionVector, m_ship2);
		m_ship2.Bounce(collisionVector * -1, m_ship1);
		m_ship1.Damage(WEAPON::SHIP);
		m_ship2.Damage(WEAPON::SHIP);
		m_pInput->GamePadVibrateRight(0, 30000, 0.5);
		m_pInput->GamePadVibrateRight(1, 30000, 0.5);
	}
	// 미사일 1과 우주선 2 사이 충돌
	if (m_torpedo1.CollidesWith(m_ship2, collisionVector)) {
		m_ship2.Damage(WEAPON::TORPEDO);
		m_torpedo1.SetVisible(false);
		m_torpedo1.SetActive(false);
		m_pInput->GamePadVibrateRight(1, 20000, 0.5);
	}
	// 미사일 2과 우주선 1 사이 충돌
	if (m_torpedo2.CollidesWith(m_ship1, collisionVector)) {
		m_ship1.Damage(WEAPON::TORPEDO);
		m_torpedo2.SetVisible(false);
		m_torpedo2.SetActive(false);
		m_pInput->GamePadVibrateRight(0, 20000, 0.5);
	}
	// 미사일 1과 행성 사이 충돌
	if (m_torpedo1.CollidesWith(m_planet, collisionVector)) {
		m_torpedo1.SetVisible(false);
		m_torpedo1.SetActive(false);
		m_pAudio->PlayCue(TORPEDO_CRASH);
	}
	// 미사일 2과 행성 사이 충돌
	if (m_torpedo2.CollidesWith(m_planet, collisionVector)) {
		m_torpedo2.SetVisible(false);
		m_torpedo2.SetActive(false);
		m_pAudio->PlayCue(TORPEDO_CRASH);
	}
	if (m_ship1.GetActive() == false && m_ship2Scored == false) {
		m_ship2Score++;
		m_ship2Scored = true;
		if (m_isRoundOver == false) {
			m_roundTimer = NSSpaceWar::ROUND_TIME;
			m_isRoundOver = true;
		}
	}
	if (m_ship2.GetActive() == false && m_ship1Scored == false) {
		m_ship1Score++;
		m_ship1Scored = true;
		if (m_isRoundOver == false) {
			m_roundTimer = NSSpaceWar::ROUND_TIME;
			m_isRoundOver = true;
		}
	}
}

void SpaceWar::Render() {
	m_pGraphics->SpriteBegin();
	m_nebula.Draw();
	m_planet.Draw();

	// 점수 
	m_fontScore.SetFontColor(NSSpaceWar::SHIP1_COLOR);
	_snprintf_s(m_buffer, NSSpaceWar::BUF_SIZE, "%d", (int)m_ship1Score);
	m_fontScore.Print(m_buffer, NSSpaceWar::SCORE1_X, NSSpaceWar::SCORE_Y);

	m_fontScore.SetFontColor(NSSpaceWar::SHIP2_COLOR);
	_snprintf_s(m_buffer, NSSpaceWar::BUF_SIZE, "%d", (int)m_ship2Score);
	m_fontScore.Print(m_buffer, NSSpaceWar::SCORE2_X, NSSpaceWar::SCORE_Y);

	// 체력바
	m_healthBar.SetX((float)NSSpaceWar::SHIP1_HEALTHBAR_X);
	m_healthBar.Set(m_ship1.GetHealth());
	m_healthBar.Draw(NSSpaceWar::SHIP1_COLOR);

	m_healthBar.SetX((float)NSSpaceWar::SHIP2_HEALTHBAR_X);
	m_healthBar.Set(m_ship2.GetHealth());
	m_healthBar.Draw(NSSpaceWar::SHIP2_COLOR);

	m_ship1.Draw();
	m_ship2.Draw();

	m_torpedo1.Draw(NSGraphics::FILTER);
	m_torpedo2.Draw(NSGraphics::FILTER);

	if (m_isMenuOn) {
		m_menu.Draw();
	}
	if (m_isCountdownOn) {
		_snprintf_s(m_buffer, NSSpaceWar::BUF_SIZE, "%d", (int)(ceil(m_countDownTimer)));
		m_fontBig.Print(m_buffer, NSSpaceWar::COUNT_DOWN_X, NSSpaceWar::COUNT_DOWN_Y);
	}

	m_pGraphics->SpriteEnd();
}

void SpaceWar::ConsoleCommand() {
	// The music control is located here because consoleCommand is always called,
	// even if the game is paused.
	if (m_paused || m_isMusicOff) {
		m_pAudio->PauseCategory("Music");
	}
	else {
		m_pAudio->ResumeCategory("Music");
	}

	m_command = m_console->GetCommand();    // 커맨드를 받아온다.
	if (m_command == "") {
		return;
	}

	if (m_command == "help") {
		m_console->Print("Console Commands:");
		m_console->Print("fps - toggle display of frames per second");
		m_console->Print("gravity off - turns off planet gravity");
		m_console->Print("gravity on - turns on planet gravity");
		m_console->Print("planet off - disables planet");
		m_console->Print("planet on - enables planet");
		m_console->Print("music on - plays music");
		m_console->Print("music off - no music");
		return;
	}
	if (m_command == "fps") {
		m_isFPSOn = !m_isFPSOn;                 // toggle display of fps
		if (m_isFPSOn) {
			m_console->Print("fps On");
		}
		else {
			m_console->Print("fps Off");
		}
	}

	if (m_command == "gravity off")	{
		m_planet.SetMass(0);
		m_console->Print("Gravity Off");
	}
	else if (m_command == "gravity on") {
		m_planet.SetMass(NSPlanet::MASS);
		m_console->Print("Gravity On");
	}
	else if (m_command == "planet off")	{
		m_planet.Disable();
		m_console->Print("Planet Off");
	}
	else if (m_command == "planet on") {
		m_planet.Able();
		m_console->Print("Planet On");
	}
	else if (m_command == "music off")
	{
		m_isMusicOff = true;
		m_console->Print("Music Off");
	}
	else if (m_command == "music on") {
		m_isMusicOff = false;
		m_console->Print("Music On");
	}
}

void SpaceWar::RoundStart() {
	// Start ships on opposite sides of planet in stable clockwise orbit
	m_ship1.SetX(GAME_WIDTH / 4 - NSShip::WIDTH);
	m_ship2.SetX(GAME_WIDTH - GAME_WIDTH / 4);
	m_ship1.SetY(GAME_HEIGHT / 2 - NSShip::HEIGHT);
	m_ship2.SetY(GAME_HEIGHT / 2);
	m_ship1.SetVelocity(VECTOR2(0, -NSShip::SPEED));
	m_ship2.SetVelocity(VECTOR2(0, NSShip::SPEED));

	m_ship1.SetDegrees(0);
	m_ship2.SetDegrees(180);
	m_ship1.Repair();
	m_ship2.Repair();
	m_countDownTimer = NSSpaceWar::COUNT_DOWN;
	m_isCountdownOn = true;
	m_isRoundOver = false;
	m_ship1Scored = false;
	m_ship2Scored = false;
}

void SpaceWar::ReleaseAll() {
	m_menuTexture.OnLostDevice();
	m_nebulaTexture.OnLostDevice();
	m_gameTexture.OnLostDevice();
	m_fontBig.OnLostDevice();
	m_fontScore.OnLostDevice();
	CGameEngine::ReleaseAll();
	return;
}

void SpaceWar::ResetAll() {
	m_menuTexture.OnResetDevice();
	m_nebulaTexture.OnResetDevice();
	m_gameTexture.OnResetDevice();
	m_fontBig.OnResetDevice();
	m_fontScore.OnResetDevice();
	CGameEngine::ResetAll();
	return;
}
