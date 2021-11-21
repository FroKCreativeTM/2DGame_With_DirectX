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
	m_fireTimer -= frameTime; // �߻簡 Ȱ��ȭ �� ������ ���� �ð�

	if (!m_visible) {
		return;
	}
	if (m_fireTimer < 0) { // �߻��� �غ� �Ǿ��ٸ� 
		m_visible = false; // ���� �̻����� ���ش�.
		m_active = false;
	}

	CImage::Update(frameTime);
	m_spriteData.x += frameTime * m_velocity.x;			 // X�࿡ ���� �ű��.
	m_spriteData.y += frameTime * m_velocity.y;			 // Y�࿡ ���� �ű��.

	// ȭ�� �𼭸� ������ ���Ѵ�
	if (m_spriteData.x > GAME_WIDTH) {
		m_spriteData.x = -NSTorpedo::WIDTH;	// ȭ�� ���������� ����� ���� �𼭸��� �ű��.
	}

	else if (m_spriteData.x < -NSTorpedo::WIDTH) {
		m_spriteData.x = GAME_WIDTH;	// ȭ�� �������� ����� ������ �𼭸��� �ű��.
	}

	if (m_spriteData.y > GAME_HEIGHT) {
		m_spriteData.y = -NSTorpedo::HEIGHT;	// ȭ�� �Ʒ������� ����� ���� �𼭸��� �ű��.
	}

	else if (m_spriteData.y < -NSTorpedo::HEIGHT) {
		m_spriteData.y = GAME_HEIGHT;	// ȭ�� �������� ����� �Ʒ��� �𼭸��� �ű��.
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
