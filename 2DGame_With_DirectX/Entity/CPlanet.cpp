#include "CPlanet.h"
#include "CPlanet.h"

CPlanet::CPlanet() : CEntity() {
    // ��ġ ����
    m_spriteData.x = NSPlanet::X;              
    m_spriteData.y = NSPlanet::Y;

    // �浹���� ���� ����
    m_radius = NSPlanet::COLLISION_RADIUS;

    // ����
    m_mass = NSPlanet::MASS;

    // ���� ������
    m_startFrame = NSPlanet::START_FRAME;    // first frame of ship animation
    m_endFrame = NSPlanet::END_FRAME;      // last frame of ship animation
    SetCurrentFrame(m_startFrame);
}