#include "SpaceWar.h"

SpaceWar::SpaceWar() {
}

SpaceWar::~SpaceWar() {
	ReleaseAll();
}

void SpaceWar::Initialize(HWND hwnd) {
	CGameEngine::Initialize(hwnd);

	// 텍스처와 이미지를 설정한다.
	if (!m_nebulaTexture.Initialize(m_pGraphics, NEBULA_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"네뷸라 텍스처 초기화 실패"));
	}

	if (!m_gameTexture.Initialize(m_pGraphics, GAME_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"게임 텍스처 초기화 실패"));
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

	if (!m_ship1.Initialize(this, NSShip::WIDTH, NSShip::HEIGHT, NSShip::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"쉽1 이미지 초기화 실패"));
	}

	m_ship1.SetFrames(NSShip::SHIP1_START_FRAME, NSShip::SHIP1_END_FRAME);
	m_ship1.SetCurrentFrame(NSShip::SHIP1_START_FRAME);
	m_ship1.SetX(GAME_WIDTH / 4);
	m_ship1.SetY(GAME_HEIGHT / 2);
	m_ship1.SetVelocity(VECTOR2(NSShip::SPEED, -NSShip::SPEED)); // VECTOR2(X, Y)

	if (!m_ship2.Initialize(this, NSShip::WIDTH, NSShip::HEIGHT, NSShip::TEXTURE_COLS, &m_gameTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"쉽2 이미지 초기화 실패"));
	}

	m_ship2.SetFrames(NSShip::SHIP2_START_FRAME, NSShip::SHIP2_END_FRAME);
	m_ship2.SetCurrentFrame(NSShip::SHIP2_START_FRAME);
	m_ship2.SetX(GAME_WIDTH - GAME_WIDTH / 4);
	m_ship2.SetY(GAME_HEIGHT / 4);
	m_ship2.SetVelocity(VECTOR2(NSShip::SPEED, NSShip::SPEED)); // VECTOR2(X, Y)

	return;
}

void SpaceWar::Update() {
	/*m_ship1.Update(m_frameTime);

	//// 우주선을 회전한다.
	//m_ship1.SetDegrees(m_ship1.GetDegrees() + m_frameTime * ROTATION_RATE);

	//// 우주선을 작게 만든다.
	//m_ship1.SetScale(m_ship1.GetScale() - m_frameTime * SCALE_RATE);
	//m_ship1.SetX(m_ship1.GetX() + m_frameTime * SHIP_SPEED);

	//// 화면 오른쪽 바깥으로 벗어나면
	//if (m_ship1.GetX() > GAME_WIDTH) {
	//	m_ship1.SetX((float)-m_ship1.GetWidth()); // 화면 왼쪽 바깥으로 옮긴다.
	//	m_ship1.SetScale(SHIP_SCALE);
	}*/

	//if (m_pInput->IsKeyDown(SHIP_RIGHT_KEY)) {
	//	m_ship1.SetX(m_ship1.GetX() + m_frameTime * SHIP_SPEED);
	//	if (m_ship1.GetX() > GAME_WIDTH) {
	//		m_ship1.SetX((float)-m_ship1.GetWidth());
	//	}
	//}
	//if (m_pInput->IsKeyDown(SHIP_LEFT_KEY)) {
	//	m_ship1.SetX(m_ship1.GetX() - m_frameTime * SHIP_SPEED);
	//	if (m_ship1.GetX() < -m_ship1.GetWidth()) {
	//		m_ship1.SetX((float)GAME_WIDTH);
	//	}
	//}
	//if (m_pInput->IsKeyDown(SHIP_UP_KEY)) {
	//	m_ship1.SetY(m_ship1.GetY() - m_frameTime * SHIP_SPEED);
	//	if (m_ship1.GetY() < -m_ship1.GetHeight()) {
	//		m_ship1.SetY((float)GAME_HEIGHT);
	//	}
	//}
	//if (m_pInput->IsKeyDown(SHIP_DOWN_KEY)) {
	//	m_ship1.SetY(m_ship1.GetY() + m_frameTime * SHIP_SPEED);
	//	if (m_ship1.GetY() > GAME_HEIGHT) {
	//		m_ship1.SetY((float)-m_ship1.GetHeight());
	//	}
	//}

	m_planet.Update(m_frameTime);
	m_ship1.Update(m_frameTime);
	m_ship2.Update(m_frameTime);
}

void SpaceWar::Ai() {
}

void SpaceWar::Collisions() {
	VECTOR2 collisionVector;

	// 우주선과 행성이 충돌 발생시
	if (m_ship1.CollidesWith(m_planet, collisionVector)) {
		// 행성에 부딪혀 튕겨져 나온다.
		m_ship1.Bounce(collisionVector, m_planet);
		m_ship1.Damage(PLANET);
	}
	if (m_ship2.CollidesWith(m_planet, collisionVector)) {
		// 행성에 부딪혀 튕겨져 나온다.
		m_ship2.Bounce(collisionVector, m_planet);
		m_ship2.Damage(PLANET);
	}
	if (m_ship1.CollidesWith(m_ship2, collisionVector)) {
		m_ship1.Bounce(collisionVector, m_ship2);
		m_ship1.Damage(SHIP);
		m_ship2.Bounce(collisionVector * -1, m_ship1);
		m_ship2.Damage(SHIP);
	}
}

void SpaceWar::Render() {
	m_pGraphics->SpriteBegin();
	m_nebula.Draw();
	m_planet.Draw();
	m_ship1.Draw();
	m_ship2.Draw();
	m_pGraphics->SpriteEnd();
}

void SpaceWar::ReleaseAll() {
	m_nebulaTexture.OnLostDevice();
	m_gameTexture.OnLostDevice();
	CGameEngine::ReleaseAll();
	return;
}

void SpaceWar::ResetAll() {
	m_nebulaTexture.OnResetDevice();
	m_gameTexture.OnResetDevice();
	CGameEngine::ResetAll();
	return;
}
