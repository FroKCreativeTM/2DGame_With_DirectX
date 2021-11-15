#include "CShip.h"

CShip::CShip() : CEntity() {
    m_spriteData.width = NSShip::WIDTH;           
    m_spriteData.height = NSShip::HEIGHT;
    m_spriteData.x = NSShip::X;                   
    m_spriteData.y = NSShip::Y;
    m_spriteData.rect.bottom = NSShip::HEIGHT;    
    m_spriteData.rect.right = NSShip::WIDTH;
    m_velocity.x = 0;                             
    m_velocity.y = 0;
    m_frameDelay = NSShip::SHIP_ANIMATION_DELAY;
    m_startFrame = NSShip::SHIP1_START_FRAME;     // first frame of ship animation
    m_endFrame = NSShip::SHIP1_END_FRAME;     // last frame of ship animation
    m_currentFrame = m_startFrame;
    m_radius = NSShip::WIDTH / 2.0;
    m_isShieldOn = false;
    m_mass = NSShip::MASS;
    m_collisionType = NSEntity::COLLISION_TYPE::CIRCLE;
}

bool CShip::Initialize(CGameEngine* pGame, 
    int width, int height, int nCols, 
    CTextureManager* pTexture) {
    m_ShieldImage.Initialize(pGame->GetGraphics(),
        width, height, nCols, pTexture);
    m_ShieldImage.SetFrames(NSShip::SHIELD_START_FRAME, NSShip::SHIELD_END_FRAME);
    m_ShieldImage.SetCurrentFrame(NSShip::SHIELD_START_FRAME);
    m_ShieldImage.SetFrameDelay(NSShip::SHIELD_ANIMATION_DELAY);
    m_ShieldImage.SetLoop(false); // 애니메이션 반복 X

    return (CEntity::Initialize(pGame, width, height, nCols, pTexture));
}

void CShip::Draw() {
    CImage::Draw(); // 우주선을 그린다.
    
    // 쉴드가 켜져 있다면 
    if (m_isShieldOn) {
        // 50%의 알파값을 가진 원을 그려준다.
        m_ShieldImage.Draw(m_spriteData, NSGraphics::ALPHA50 & m_colorFilter);
    }
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
        m_pAudio->PlayCue(BEEP1);
    }
    else if (m_spriteData.x < 0) { // 왼쪽 모서리 충돌
        m_spriteData.x = 0;
        m_velocity.x = -m_velocity.x;
        m_pAudio->PlayCue(BEEP2);
    }

    // 화면 아래쪽 모서리에 충돌
    if (m_spriteData.y > GAME_HEIGHT - NSShip::HEIGHT * GetScale()) {
        m_spriteData.y = GAME_HEIGHT - NSShip::HEIGHT * GetScale();
        m_velocity.y = -m_velocity.y;
        m_pAudio->PlayCue(BEEP3);
    }

    else if (m_spriteData.y < 0) { // 왼쪽 모서리 충돌
        m_spriteData.y = 0;
        m_velocity.y = -m_velocity.y;
        m_pAudio->PlayCue(BEEP4);
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
}

void CShip::Damage(int weapon) {
    m_isShieldOn = true;
}
