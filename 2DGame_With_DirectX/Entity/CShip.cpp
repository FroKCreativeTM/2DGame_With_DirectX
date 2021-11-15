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
    m_ShieldImage.SetLoop(false); // �ִϸ��̼� �ݺ� X

    return (CEntity::Initialize(pGame, width, height, nCols, pTexture));
}

void CShip::Draw() {
    CImage::Draw(); // ���ּ��� �׸���.
    
    // ���尡 ���� �ִٸ� 
    if (m_isShieldOn) {
        // 50%�� ���İ��� ���� ���� �׷��ش�.
        m_ShieldImage.Draw(m_spriteData, NSGraphics::ALPHA50 & m_colorFilter);
    }
}

void CShip::Update(float frameTime) {
    CEntity::Update(frameTime);


    m_spriteData.angle += frameTime * NSShip::ROTATION_RATE; // ���ּ��� ȸ�� ��Ų��.

    m_spriteData.x += frameTime * m_velocity.x; // x���� ���� ���ּ��� �̵���ŵ�ϴ�.
    m_spriteData.y += frameTime * m_velocity.y; // y���� ���� ���ּ��� �̵���ŵ�ϴ�.

    // �����κ��� ƨ�� ������ �����.
    // ȭ�� ������ �𼭸��� �浹�ϴ� ���
    if (m_spriteData.x > GAME_WIDTH - NSShip::WIDTH * GetScale()) {
        // ȭ�� ������ �𼭸��� ��ġ��Ų��.
        m_spriteData.x = GAME_WIDTH - NSShip::WIDTH * GetScale();
        m_velocity.x = -m_velocity.x;
        m_pAudio->PlayCue(BEEP1);
    }
    else if (m_spriteData.x < 0) { // ���� �𼭸� �浹
        m_spriteData.x = 0;
        m_velocity.x = -m_velocity.x;
        m_pAudio->PlayCue(BEEP2);
    }

    // ȭ�� �Ʒ��� �𼭸��� �浹
    if (m_spriteData.y > GAME_HEIGHT - NSShip::HEIGHT * GetScale()) {
        m_spriteData.y = GAME_HEIGHT - NSShip::HEIGHT * GetScale();
        m_velocity.y = -m_velocity.y;
        m_pAudio->PlayCue(BEEP3);
    }

    else if (m_spriteData.y < 0) { // ���� �𼭸� �浹
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
