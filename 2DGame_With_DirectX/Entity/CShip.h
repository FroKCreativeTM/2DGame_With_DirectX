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
    const int           TEXTURE_COLS = 8;    
    const int           SHIP1_START_FRAME = 0;      
    const int           SHIP1_END_FRAME = 3;        
    const int           SHIP2_START_FRAME = 8;      
    const int           SHIP2_END_FRAME = 11;       
    const float         SHIP_ANIMATION_DELAY = 0.2f;
    const int           SHIELD_START_FRAME = 24;    
    const int           SHIELD_END_FRAME = 27;      
    const float         SHIELD_ANIMATION_DELAY = 0.1f; 
}

class CShip : public CEntity {
private : 
    bool        m_isShieldOn;
    CImage      m_ShieldImage;
public : 
    CShip();
    virtual bool Initialize(CGameEngine* pGame, int width, int height, int nCols,
        CTextureManager* pTexture);
    void Draw();
    void Update(float frameTime);
    void Damage(int weapon);
};

