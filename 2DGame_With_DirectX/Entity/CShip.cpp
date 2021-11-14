#include "CShip.h"

CShip::CShip() {
    m_spriteData.width = NSShip::WIDTH;           
    m_spriteData.height = NSShip::HEIGHT;
    m_spriteData.x = NSShip::X;                   
    m_spriteData.y = NSShip::Y;
    m_spriteData.rect.bottom = NSShip::HEIGHT;    
    m_spriteData.rect.right = NSShip::WIDTH;
    m_velocity.x = 0;                             
    m_velocity.y = 0;                             
    m_frameDelay = NSShip::SHIP_ANIMATION_DELAY;
    m_startFrame = NSShip::SHIP_START_FRAME;      
    m_endFrame = NSShip::SHIP_END_FRAME;      
    m_currentFrame = m_startFrame;
    m_radius = NSShip::WIDTH / 2.0;
    m_collisionType = NSEntity::COLLISION_TYPE::CIRCLE;
}

void CShip::Update(float frameTime) {
    CEntity::Update(frameTime);

    m_spriteData.angle += frameTime * NSShip::ROTATION_RATE; // 우주선을 회전 시킨다.

    m_spriteData.x += frameTime * m_velocity.x; // x축을 따라 우주선을 이동시킵니다.
    m_spriteData.y += frameTime * m_velocity.y; // y축을 따라 우주선을 이동시킵니다.

    // 벽으로부터 튕겨 나오게 만든다.
    // 화면 오른쪽 모서리에 충돌하는 경우
    if (m_spriteData.x > GAME_WIDTH - NSShip::WIDTH * GetScale()) {
        // 화면 오른쪽 모서리에 위치시킨다.
        m_spriteData.x = GAME_WIDTH - NSShip::WIDTH * GetScale();
        m_velocity.x = -m_velocity.x;
    }
    else if (m_spriteData.x < 0) { // 왼쪽 모서리 충돌
        m_spriteData.x = 0;
        m_velocity.x = -m_velocity.x;
    }

    // 화면 아래쪽 모서리에 충돌
    if (m_spriteData.y > GAME_HEIGHT - NSShip::HEIGHT * GetScale()) {
        m_spriteData.y = GAME_HEIGHT - NSShip::HEIGHT * GetScale();
        m_velocity.y = -m_velocity.y;
    }
    else if (m_spriteData.y < 0) { // 왼쪽 모서리 충돌
        m_spriteData.y = 0;
        m_velocity.y = -m_velocity.y;
    }
}
