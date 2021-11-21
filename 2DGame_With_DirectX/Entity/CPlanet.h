#pragma once
#include "CEntity.h"

namespace NSPlanet {
    const int   WIDTH = 128;                // image width
    const int   HEIGHT = 128;               // image height
    const int   COLLISION_RADIUS = 120 / 2;   // for circular collision
    const int   X = GAME_WIDTH / 2 - WIDTH / 2; // location on screen
    const int   Y = GAME_HEIGHT / 2 - HEIGHT / 2;
    const float MASS = 1.0e14f;             // mass
    const int   TEXTURE_COLS = 2;           // texture has 1 column
    const int   START_FRAME = 1;            // starts at frame 0
    const int   END_FRAME = 1;              // no animation
}


class CPlanet : public CEntity {
public : 
	CPlanet();
    void Disable() { m_visible = false; m_active = false; }
    void Able() { m_visible = true; m_active = true; }
};

