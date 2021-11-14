#pragma once

#include "CEntity.h"

// Ship 클래스의 전반적인 정보를 다루는 네임스페이스
namespace NSShip
{
    const int           WIDTH = 32;                   
    const int           HEIGHT = 32;                  
    const int           X = GAME_WIDTH / 2 - WIDTH / 2;  
    const int           Y = GAME_HEIGHT / 2 - HEIGHT / 2;
    const float         ROTATION_RATE = (float)PI / 4; 
    const float         SPEED = 100;                
    const float         MASS = 300.0f;              
    const int           TEXTURE_COLS = 2;           
    const int           SHIP_START_FRAME = 0;       
    const int           SHIP_END_FRAME = 3;         
    const float         SHIP_ANIMATION_DELAY = 0.2f; 
}

class CShip : public CEntity {
public : 
    CShip();
    void Update(float frameTime);
};

