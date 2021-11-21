#pragma once

#include "CEntity.h"

// Ship 클래스의 전반적인 정보를 다루는 네임스페이스
namespace NSShip {
    const int           WIDTH = 32;                   
    const int           HEIGHT = 32;                  
    const int           X = GAME_WIDTH / 2 - WIDTH / 2;  
    const int           Y = GAME_HEIGHT / 2 - HEIGHT / 2;
    const float         ROTATION_RATE = (float)PI / 4; 
    const float         SPEED = 100;                
    const float         MASS = 300.0f;              
    enum class          DIRECTION { NONE, LEFT, RIGHT };
    const int           TEXTURE_COLS = 8;    

    // 쉽 
    const int           SHIP1_START_FRAME = 0;      
    const int           SHIP1_END_FRAME = 3;        

    const int           SHIP2_START_FRAME = 8;      
    const int           SHIP2_END_FRAME = 11;       

    const float         SHIP_ANIMATION_DELAY = 0.2f;

    // 폭발 
    const int           EXPLOSION_START_FRAME = 32;    
    const int           EXPLOSION_END_FRAME = 39;      
    const float         EXPLOSION_ANIMATION_DELAY = 0.2f; 

    // 엔진
    const int           ENGINE_START_FRAME = 16;
    const int           ENGINE_END_FRAME = 19;
    const float         ENGINE_ANIMATION_DELAY = 0.1f;

    // 쉴드
    const int           SHIELD_START_FRAME = 24;
    const int           SHIELD_END_FRAME = 27;
    const float         SHIELD_ANIMATION_DELAY = 0.1f;

    // 데미지 값
    const float         TORPEDO_DAMAGE = 46;
    const float         SHIP_DAMAGE = 10;
}

class CShip : public CEntity {
private : 
    float               m_oldX, m_oldY, m_oldAngle;
    float               m_rotation;
    NSShip::DIRECTION   m_direction;
    float               m_explosionTimer;
    bool                m_isExplosionOn;
    bool                m_isEngineOn;
    bool                m_isShieldOn;
    CImage              m_ExplosionImage;
    CImage              m_EngineImage;
    CImage              m_ShieldImage;

public : 
    CShip();
    virtual bool Initialize(CGameEngine* pGame, int width, int height, int nCols,
        CTextureManager* pTexture);
    void Draw();
    void Update(float frameTime);
    void Damage(int weapon);

    void ToOldPosition() {
        m_spriteData.x = m_oldX;
        m_spriteData.y = m_oldY;
        m_spriteData.angle = m_oldAngle;
        m_rotation = 0.0f;
    }

    float GetRotation() const { return m_rotation; }
    bool GetEngineOn() const { return m_isEngineOn; }
    bool GetExplosionOn() const { return m_isExplosionOn; }
    bool GetShieldOn() const { return m_isShieldOn; }
    void SetEngineOn(bool engine) { m_isEngineOn = engine; }
    void SetShieldOn(bool shield) { m_isShieldOn = shield; }
    void SetMass(float mass) { m_mass = mass; }
    void SetRotation(float rotation) { m_rotation = rotation; }
    void Rotate(NSShip::DIRECTION dir) { m_direction = dir; }
    void Explode();
    void Repair();
};

