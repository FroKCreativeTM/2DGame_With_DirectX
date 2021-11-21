#include "CShip.h"

CShip::CShip() : CEntity() {
    m_spriteData.width = NSShip::WIDTH;           
    m_spriteData.height = NSShip::HEIGHT;

    m_spriteData.x = NSShip::X;                   
    m_spriteData.y = NSShip::Y;

    m_spriteData.rect.bottom = NSShip::HEIGHT;    
    m_spriteData.rect.right = NSShip::WIDTH;

    m_oldX = NSShip::X; 
    m_oldY = NSShip::Y;
    m_oldAngle = 0.0f;
    m_rotation = 0.0f;

    m_velocity.x = 0;                             
    m_velocity.y = 0;

    m_frameDelay = NSShip::SHIP_ANIMATION_DELAY;
    m_startFrame = NSShip::SHIP1_START_FRAME;     // first frame of ship animation
    m_endFrame = NSShip::SHIP1_END_FRAME;     // last frame of ship animation
    m_currentFrame = m_startFrame;

    m_radius = NSShip::WIDTH / 2.0;

    m_collisionType = NSEntity::COLLISION_TYPE::CIRCLE;

    m_direction = NSShip::DIRECTION::NONE;

    m_isShieldOn = false;
    m_isEngineOn = false;
    m_isExplosionOn = false;

    m_mass = NSShip::MASS;
}

bool CShip::Initialize(CGameEngine* pGame, 
    int width, int height, int nCols, 
    CTextureManager* pTexture) {
    // 쉴드
    m_ShieldImage.Initialize(pGame->GetGraphics(),
        width, height, nCols, pTexture);
    m_ShieldImage.SetFrames(NSShip::SHIELD_START_FRAME, NSShip::SHIELD_END_FRAME);
    m_ShieldImage.SetCurrentFrame(NSShip::SHIELD_START_FRAME);
    m_ShieldImage.SetFrameDelay(NSShip::SHIELD_ANIMATION_DELAY);
    m_ShieldImage.SetLoop(false); // 애니메이션 반복 X

    // 엔진
    m_EngineImage.Initialize(pGame->GetGraphics(), width, height, nCols, pTexture);
    m_EngineImage.SetFrames(NSShip::ENGINE_START_FRAME, NSShip::ENGINE_END_FRAME);
    m_EngineImage.SetCurrentFrame(NSShip::ENGINE_START_FRAME);
    m_EngineImage.SetFrameDelay(NSShip::ENGINE_ANIMATION_DELAY);

    // 폭파
    m_ExplosionImage.Initialize(pGame->GetGraphics(), width, height, nCols, pTexture);
    m_ExplosionImage.SetFrames(NSShip::EXPLOSION_START_FRAME, NSShip::EXPLOSION_END_FRAME);
    m_ExplosionImage.SetCurrentFrame(NSShip::EXPLOSION_START_FRAME);
    m_ExplosionImage.SetFrameDelay(NSShip::EXPLOSION_ANIMATION_DELAY);
    m_ExplosionImage.SetLoop(false); // 애니메이션 반복 X

    return (CEntity::Initialize(pGame, width, height, nCols, pTexture));
}

void CShip::Draw() {
    if (m_isExplosionOn) {
        m_ExplosionImage.Draw(m_spriteData); // 폭발을 그린다.
    }
    else {
        CImage::Draw(); // 우주선을 그린다.

        // 엔진이 켜져 있다면 
        if (m_isEngineOn) {
            // 엔진 로켓을 그린다
            m_EngineImage.Draw(m_spriteData);
        }
        // 쉴드가 켜져 있다면 
        if (m_isShieldOn) {
            // 50%의 알파값을 가진 원을 그려준다.
            m_ShieldImage.Draw(m_spriteData, NSGraphics::ALPHA50 & m_colorFilter);
        }
    }
}

void CShip::Update(float frameTime) {
    if (m_isExplosionOn) {
        m_ExplosionImage.Update(frameTime);
        if (m_ShieldImage.GetAnimationComplete())
        {
            m_isExplosionOn = false;
            m_visible = false;
            m_ExplosionImage.SetAnimationComplete(false);
            m_ExplosionImage.SetCurrentFrame(NSShip::EXPLOSION_START_FRAME);
        }
    }
    if (m_isShieldOn)
    {
        m_ShieldImage.Update(frameTime);
        if (m_ShieldImage.GetAnimationComplete())
        {
            m_isShieldOn = false;
            m_ShieldImage.SetAnimationComplete(false);
        }
    }
    if (m_isEngineOn) {
        m_velocity.x += (float)cos(m_spriteData.angle) * NSShip::SPEED * frameTime;
        m_velocity.y += (float)sin(m_spriteData.angle) * NSShip::SPEED * frameTime;
        m_EngineImage.Update(frameTime);
    }

    CEntity::Update(frameTime);

    m_oldX = m_spriteData.x;
    m_oldY = m_spriteData.y;
    m_oldAngle = m_spriteData.angle;

    switch (m_direction)
    {
    case NSShip::DIRECTION::LEFT:
        m_rotation -= frameTime * NSShip::ROTATION_RATE;
        break;
    case NSShip::DIRECTION::RIGHT:
        m_rotation += frameTime * NSShip::ROTATION_RATE;
        break;
    }

    m_spriteData.angle += frameTime * m_rotation; // 우주선을 회전 시킨다.
    m_spriteData.x += frameTime * m_velocity.x; // x축을 따라 우주선을 이동시킵니다.
    m_spriteData.y += frameTime * m_velocity.y; // y축을 따라 우주선을 이동시킵니다.

    // 화면 오른쪽 모서리에 충돌하는 경우
    if (m_spriteData.x > GAME_WIDTH) {
        // 화면 왼쪽 모서리에 위치시킨다.
        m_spriteData.x = -NSShip::WIDTH;
    }
    else if (m_spriteData.x < -NSShip::WIDTH) { // 왼쪽 모서리 충돌
        m_spriteData.x = GAME_WIDTH;
    }

    // 화면 아래쪽 모서리에 충돌
    if (m_spriteData.y > GAME_HEIGHT) {
        m_spriteData.y = -NSShip::HEIGHT;
    }

    else if (m_spriteData.y < -NSShip::HEIGHT) { // 왼쪽 모서리 충돌
        m_spriteData.y = GAME_HEIGHT;
    }    
}

void CShip::Damage(WEAPON weapon) {
    if (m_isShieldOn) {
        return;  // 쉴드가 쳐져 있다면 행동하지 않는다.
    }

    switch (weapon)
    {
    case WEAPON::TORPEDO:
        m_pAudio->PlayCue(TORPEDO_HIT);
        m_health -= NSShip::TORPEDO_DAMAGE;
        break;
    case WEAPON::SHIP:
        m_pAudio->PlayCue(COLLIDE);
        m_health -= NSShip::SHIP_DAMAGE;
        break;
    case WEAPON::PLANET:
        m_health = 0;
        break;
    }

    if (m_health <= 0) {
        Explode();
    }
    else {
        m_isShieldOn = true;
    }
}

void CShip::Explode() {
    m_pAudio->PlayCue(EXPLODE);
    m_active = false;
    m_health = 0;
    m_isExplosionOn = true;
    m_isEngineOn = false;
    m_isShieldOn = false;
    m_velocity.x = 0;
    m_velocity.y = 0;
}

void CShip::Repair() {
    m_active = true;
    m_health = FULL_HEALTH;
    m_isExplosionOn = false;
    m_isEngineOn = false;
    m_isShieldOn = false;
    m_rotation = 0.0f;
    m_direction = NSShip::DIRECTION::NONE;
    m_visible = true;
}
