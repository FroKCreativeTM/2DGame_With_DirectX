#include "CTorpedo.h"

CTorpedo::CTorpedo() {
	m_active = false;
	m_spriteData.width = NSTorpedo::WIDTH;
	m_spriteData.height = NSTorpedo::HEIGHT;
	m_spriteData.rect.bottom = NSTorpedo::HEIGHT;
	m_spriteData.rect.right = NSTorpedo::WIDTH;
	m_cols = NSTorpedo::TEXTURE_COLS;
	m_frameDelay = NSTorpedo::ANIMATION_DELAY;
	m_startFrame = NSTorpedo::START_FRAME;
	m_endFrame = NSTorpedo::END_FRAME;
	m_currentFrame = NSTorpedo::START_FRAME;
	m_radius = NSTorpedo::COLLISION_RADIUS;
	m_visible = false;
	m_fireTimer = 0.0f;
	m_mass = NSTorpedo::MASS;
	m_collisionType = NSEntity::COLLISION_TYPE::CIRCLE;
}

void CTorpedo::Update(float frameTime) {
	m_fireTimer -= frameTime; // 발사가 활성화 될 때까지 남은 시간

	if (!m_visible) {
		return;
	}
	if (m_fireTimer < 0) { // 발사할 준비가 되었다면 
		m_visible = false; // 기존 미사일은 없앤다.
		m_active = false;
	}

	CImage::Update(frameTime);
	m_spriteData.x += frameTime * m_velocity.x;			 // X축에 따라 옮긴다.
	m_spriteData.y += frameTime * m_velocity.y;			 // Y축에 따라 옮긴다.

	// 화면 모서리 주위를 감싼다
	if (m_spriteData.x > GAME_WIDTH) {
		m_spriteData.x = -NSTorpedo::WIDTH;	// 화면 오른쪽으로 벗어나면 왼쪽 모서리로 옮긴다.
	}

	else if (m_spriteData.x < -NSTorpedo::WIDTH) {
		m_spriteData.x = GAME_WIDTH;	// 화면 왼쪽으로 벗어나면 오른쪽 모서리로 옮긴다.
	}

	if (m_spriteData.y > GAME_HEIGHT) {
		m_spriteData.y = -NSTorpedo::HEIGHT;	// 화면 아래쪽으로 벗어나면 위쪽 모서리로 옮긴다.
	}

	else if (m_spriteData.y < -NSTorpedo::HEIGHT) {
		m_spriteData.y = GAME_HEIGHT;	// 화면 위쪽으로 벗어나면 아래쪽 모서리로 옮긴다.
	}
}

void CTorpedo::Fire(CEntity* ship) {
	if (m_fireTimer <= 0.0f) {
		m_velocity.x = (float)cos(ship->GetRadians()) * NSTorpedo::SPEED;
		m_velocity.y = (float)sin(ship->GetRadians()) * NSTorpedo::SPEED;
		m_spriteData.x = ship->GetCenterX() - m_spriteData.width / 2;
		m_spriteData.y = ship->GetCenterY() - m_spriteData.height / 2;
		m_visible = true;
		m_active = true;
		m_fireTimer = NSTorpedo::FIRE_DELAY;
		m_pAudio->PlayCue(TORPEDO_FIRE);
	}
}
