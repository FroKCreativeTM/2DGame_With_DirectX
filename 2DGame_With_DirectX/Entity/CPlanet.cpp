#include "CPlanet.h"
#include "CPlanet.h"

CPlanet::CPlanet() : CEntity() {
    // 위치 지정
    m_spriteData.x = NSPlanet::X;              
    m_spriteData.y = NSPlanet::Y;

    // 충돌점을 위한 각도
    m_radius = NSPlanet::COLLISION_RADIUS;

    // 질량
    m_mass = NSPlanet::MASS;

    // 시작 프레임
    m_startFrame = NSPlanet::START_FRAME;    // first frame of ship animation
    m_endFrame = NSPlanet::END_FRAME;      // last frame of ship animation
    SetCurrentFrame(m_startFrame);
}