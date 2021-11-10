#include "SpaceWar.h"

SpaceWar::SpaceWar() {
}

SpaceWar::~SpaceWar() {
	ReleaseAll();
}

void SpaceWar::Initialize(HWND hwnd) {
	CGameEngine::Initialize(hwnd);

	// �ؽ�ó�� �̹����� �����Ѵ�.
	if (!m_nebulaTexture.Initialize(m_pGraphics, NEBULA_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�׺�� �ؽ�ó �ʱ�ȭ ����"));
	}

	if (!m_planetTexture.Initialize(m_pGraphics, PLANET_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�༺ �ؽ�ó �ʱ�ȭ ����"));
	}

	if (!m_shipTexture.Initialize(m_pGraphics, SHIP_IMAGE)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�� �ؽ�ó �ʱ�ȭ ����"));
	}

	if (!m_nebula.Initialize(m_pGraphics, 0, 0, 0, &m_nebulaTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�׺�� �̹��� �ʱ�ȭ ����"));
	}

	if (!m_planet.Initialize(m_pGraphics, 0, 0, 0, &m_planetTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�༺ �̹��� �ʱ�ȭ ����"));
	}

	// �༺�� ȭ�� �߾ӿ� ��ġ�Ѵ�.
	m_planet.SetX(GAME_WIDTH * 0.5f - m_planet.GetWidth() * 0.5f);
	m_planet.SetY(GAME_HEIGHT * 0.5f - m_planet.GetHeight() * 0.5f);

	if (!m_ship.Initialize(m_pGraphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &m_shipTexture)) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"�� �̹��� �ʱ�ȭ ����"));
	}

	m_ship.SetX(GAME_WIDTH / 4);
	m_ship.SetY(GAME_HEIGHT / 4);

	m_ship.SetFrames(SHIP_START_FRAME, SHIP_END_FRAME);
	m_ship.SetCurrentFrame(SHIP_START_FRAME);
	m_ship.SetFrameDelay(SHIP_ANIMATION_DELAY);
	m_ship.SetDegrees(45.0f);

	return;
}

void SpaceWar::Update() {
	m_ship.Update(m_frameTime);
}

void SpaceWar::Ai() {
}

void SpaceWar::Collisions() {
}

void SpaceWar::Render() {
	m_pGraphics->SpriteBegin();
	m_nebula.Draw();
	m_planet.Draw();
	m_ship.Draw();
	m_pGraphics->SpriteEnd();
}

void SpaceWar::ReleaseAll() {
	m_planetTexture.OnLostDevice();
	m_nebulaTexture.OnLostDevice();
	m_shipTexture.OnLostDevice();
	CGameEngine::ReleaseAll();
	return;
}

void SpaceWar::ResetAll() {
	m_nebulaTexture.OnResetDevice();
	m_planetTexture.OnResetDevice();
	m_shipTexture.OnResetDevice();
	CGameEngine::ResetAll();
	return;
}
