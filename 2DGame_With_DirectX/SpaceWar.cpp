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

	// DirectX ��Ʈ�� �ʱ�ȭ�Ѵ�.
	m_fontBig.Initialize(m_pGraphics, NSSpaceWar::FONT_BIG_SIZE, false, false, NSSpaceWar::FONT);
	m_fontBig.SetFontColor(NSSpaceWar::FONT_COLOR);
	m_fontScore.Initialize(m_pGraphics, NSSpaceWar::FONT_SCORE_SIZE, false, false, NSSpaceWar::FONT);

	// �ؽ�ó�� �̹����� �����Ѵ�.
	if (!m_menuTexture.Initialize(m_pGraphics, MENU_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�޴� �ؽ�ó �ʱ�ȭ ����"));
	}

	if (!m_nebulaTexture.Initialize(m_pGraphics, NEBULA_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�׺�� �ؽ�ó �ʱ�ȭ ����"));
	}

	if (!m_gameTexture.Initialize(m_pGraphics, GAME_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"���� �ؽ�ó �ʱ�ȭ ����"));
	}

	if (!m_menu.Initialize(m_pGraphics, 0, 0, 0, &m_menuTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�޴� �̹��� �ʱ�ȭ ����"));
	}

	if (!m_nebula.Initialize(m_pGraphics, 0, 0, 0, &m_nebulaTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�׺�� �̹��� �ʱ�ȭ ����"));
	}

	if (!m_planet.Initialize(this, NSPlanet::WIDTH, NSPlanet::HEIGHT, 
		NSPlanet::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�༺ �̹��� �ʱ�ȭ ����"));
	}

	if (!m_ship1.Initialize(this, NSShip::WIDTH, NSShip::HEIGHT, 
		NSShip::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"��1 �̹��� �ʱ�ȭ ����"));
	}

	m_ship1.SetFrames(NSShip::SHIP1_START_FRAME, NSShip::SHIP1_END_FRAME);
	m_ship1.SetCurrentFrame(NSShip::SHIP1_START_FRAME);
	m_ship1.SetColorFilter(SETCOLOR_ARGB(255, 230, 230, 255));
	m_ship1.SetMass(NSShip::MASS);

	if (!m_ship2.Initialize(this, NSShip::WIDTH, NSShip::HEIGHT, NSShip::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"��2 �̹��� �ʱ�ȭ ����"));
	}

	m_ship2.SetFrames(NSShip::SHIP2_START_FRAME, NSShip::SHIP2_END_FRAME);
	m_ship2.SetCurrentFrame(NSShip::SHIP2_START_FRAME);
	m_ship2.SetColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));
	m_ship2.SetMass(NSShip::MASS);

	// �̻��� �ʱ�ȭ
	if (!m_torpedo1.Initialize(this, NSTorpedo::WIDTH, NSTorpedo::HEIGHT,
		NSTorpedo::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�̻���1 �̹��� �ʱ�ȭ ����"));
	}

	m_torpedo1.SetFrames(NSTorpedo::START_FRAME, NSTorpedo::END_FRAME);
	m_torpedo1.SetCurrentFrame(NSTorpedo::START_FRAME);
	m_torpedo1.SetColorFilter(SETCOLOR_ARGB(255, 128, 128, 255));

	if (!m_torpedo2.Initialize(this, NSTorpedo::WIDTH, NSTorpedo::HEIGHT,
		NSTorpedo::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�̻���2 �̹��� �ʱ�ȭ ����"));
	}

	m_torpedo2.SetFrames(NSTorpedo::START_FRAME, NSTorpedo::END_FRAME);
	m_torpedo2.SetCurrentFrame(NSTorpedo::START_FRAME);
	m_torpedo2.SetColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));

	// ü�� ǥ����
	m_healthBar.Initialize(m_pGraphics, &m_gameTexture, 0, NSSpaceWar::HEALTHBAR_Y,
		2.0f, NSGraphics::WHITE);

	// ���ּ��� �ð� ���� �˵��� �����̴� �༺�� �ݴ����� �����ϰ� �����
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
			// ������ �����ٸ�
			if (m_pInput->IsKeyDown(SHIP1_FORWARD_KEY) || m_pInput->GetGamepadDPadUp(0)) {
				m_ship1.SetEngineOn(true);
				m_pAudio->PlayCue(ENGINE1);
			}
			else {
				m_ship1.SetEngineOn(false);
				m_pAudio->StopCue(ENGINE1);
			}
			m_ship1.Rotate(NSShip::DIRECTION::NONE);

			// �������� ������ �����ٸ�
			if (m_pInput->IsKeyDown(SHIP1_LEFT_KEY) || m_pInput->GetGamepadDPadLeft(0)) {
				m_ship1.Rotate(NSShip::DIRECTION::LEFT);
			}
			// ���������� ������ �����ٸ�
			if (m_pInput->IsKeyDown(SHIP1_RIGHT_KEY) || m_pInput->GetGamepadDPadRight(0)) {
				m_ship1.Rotate(NSShip::DIRECTION::RIGHT);
			}
			// ���������� ������ �����ٸ�
			if (m_pInput->IsKeyDown(SHIP1_FIRE_KEY) || m_pInput->GetGamepadA(0)) {
				m_torpedo1.Fire(&m_ship1);
			}
		}
		if (m_ship2.GetActive()) {
			// ������ �����ٸ�
			if (m_pInput->IsKeyDown(SHIP2_FORWARD_KEY) || m_pInput->GetGamepadDPadUp(0)) {
				m_ship2.SetEngineOn(true);
				m_pAudio->PlayCue(ENGINE2);
			}
			else {
				m_ship2.SetEngineOn(false);
				m_pAudio->StopCue(ENGINE2);
			}
			m_ship2.Rotate(NSShip::DIRECTION::NONE);

			// �������� ������ �����ٸ�
			if (m_pInput->IsKeyDown(SHIP2_LEFT_KEY) || m_pInput->GetGamepadDPadLeft(0)) {
				m_ship2.Rotate(NSShip::DIRECTION::LEFT);
			}
			// ���������� ������ �����ٸ�
			if (m_pInput->IsKeyDown(SHIP2_RIGHT_KEY) || m_pInput->GetGamepadDPadRight(0)) {
				m_ship2.Rotate(NSShip::DIRECTION::RIGHT);
			}
			// ���������� ������ �����ٸ�
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

	// ��� ��ü ����
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

	// ���ּ� 1�� �༺�� �浹 �߻���
	if (m_ship1.CollidesWith(m_planet, collisionVector)) {
		// �༺�� �ε��� ƨ���� ���´�.
		m_ship1.ToOldPosition();
		m_ship1.Damage(WEAPON::PLANET);
		m_pInput->GamePadVibrateLeft(0, 65535, 1.0);
	}
	// ���ּ� 2�� �༺�� �浹 �߻���
	if (m_ship2.CollidesWith(m_planet, collisionVector)) {
		// �༺�� �ε��� ƨ���� ���´�.
		m_ship2.ToOldPosition();
		m_ship2.Damage(WEAPON::PLANET);
		m_pInput->GamePadVibrateLeft(1, 65535, 1.0);
	}
	// ���ּ� 1�� 2 �浹 �߻���
	if (m_ship1.CollidesWith(m_ship2, collisionVector)) {
		m_ship1.Bounce(collisionVector, m_ship2);
		m_ship2.Bounce(collisionVector * -1, m_ship1);
		m_ship1.Damage(WEAPON::SHIP);
		m_ship2.Damage(WEAPON::SHIP);
		m_pInput->GamePadVibrateRight(0, 30000, 0.5);
		m_pInput->GamePadVibrateRight(1, 30000, 0.5);
	}
	// �̻��� 1�� ���ּ� 2 ���� �浹
	if (m_torpedo1.CollidesWith(m_ship2, collisionVector)) {
		m_ship2.Damage(WEAPON::TORPEDO);
		m_torpedo1.SetVisible(false);
		m_torpedo1.SetActive(false);
		m_pInput->GamePadVibrateRight(1, 20000, 0.5);
	}
	// �̻��� 2�� ���ּ� 1 ���� �浹
	if (m_torpedo2.CollidesWith(m_ship1, collisionVector)) {
		m_ship1.Damage(WEAPON::TORPEDO);
		m_torpedo2.SetVisible(false);
		m_torpedo2.SetActive(false);
		m_pInput->GamePadVibrateRight(0, 20000, 0.5);
	}
	// �̻��� 1�� �༺ ���� �浹
	if (m_torpedo1.CollidesWith(m_planet, collisionVector)) {
		m_torpedo1.SetVisible(false);
		m_torpedo1.SetActive(false);
		m_pAudio->PlayCue(TORPEDO_CRASH);
	}
	// �̻��� 2�� �༺ ���� �浹
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

	// ���� 
	m_fontScore.SetFontColor(NSSpaceWar::SHIP1_COLOR);
	_snprintf_s(m_buffer, NSSpaceWar::BUF_SIZE, "%d", (int)m_ship1Score);
	m_fontScore.Print(m_buffer, NSSpaceWar::SCORE1_X, NSSpaceWar::SCORE_Y);

	m_fontScore.SetFontColor(NSSpaceWar::SHIP2_COLOR);
	_snprintf_s(m_buffer, NSSpaceWar::BUF_SIZE, "%d", (int)m_ship2Score);
	m_fontScore.Print(m_buffer, NSSpaceWar::SCORE2_X, NSSpaceWar::SCORE_Y);

	// ü�¹�
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

	m_command = m_console->GetCommand();    // Ŀ�ǵ带 �޾ƿ´�.
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
