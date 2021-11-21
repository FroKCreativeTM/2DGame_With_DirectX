#pragma once
#include "CEntity.h"

namespace NSTorpedo {
    const int           WIDTH = 32;
    const int           HEIGHT = 32;
    const int           COLLISION_RADIUS = 4;    // 원형 충돌
    const float         SPEED = 200;
    const float         MASS = 300.0f;
    const float         FIRE_DELAY = 4.0f;
    const int           TEXTURE_COLS = 8;
    const int           START_FRAME = 40;
    const int           END_FRAME = 43;
    const float         ANIMATION_DELAY = 0.1f;
}

class CTorpedo :
    public CEntity {
private : 
    float m_fireTimer;
public : 
    CTorpedo();

    void Update(float frameTime);
    float GetMass() const { return NSTorpedo::MASS; }
    void Fire(CEntity* ship);
};

