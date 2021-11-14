#include "CEntity.h"

// ���� �Ÿ� �������� �̿��ؼ� �� ���� �浹�� �����Ѵ�.
bool CEntity::CollideCircle(CEntity& entity, 
    VECTOR2& collisionVector)
{
    // �� �߽� ������ ���̸� ���Ѵ�.
    VECTOR2 distSquared = *GetCenter() - *entity.GetCenter();
    distSquared.x = distSquared.x * distSquared.x;
    distSquared.y = distSquared.y * distSquared.y;

    // �� ���� �������� ���� ���Ѵ�.
    FLOAT sumRadiiSquared = (m_radius * GetScale()) +
        (entity.m_radius * entity.GetScale());
    sumRadiiSquared *= sumRadiiSquared;

    if (distSquared.x + distSquared.y <= sumRadiiSquared) {
        // ��ü���� �浹�� ���
        collisionVector = *entity.GetCenter() - *GetCenter(); // �浹�� ���� ���Ͱ�
        return true;
    }
    return false; // �浹���� ����.
}

// A�� ������ �𼭸� >= B�� ���� �𼭸�
// A�� ���� �𼭸� <= B�� ������ �𼭸�
// A�� �Ʒ��� �𼭸� >= B�� ���� �𼭸�
// A�� ���� �𼭸� <= B�� �Ʒ��� �𼭸�
bool CEntity::CollideBox(CEntity& entity, 
    VECTOR2& collisionVector) {
    if (!m_active || !entity.GetActive()) { // �� �� �ϳ��� Ȱ��ȭ�� �� �Ǿ��ִٸ�
        return false;
    }

    if ((GetCenterX() + m_edge.right * GetScale() >= entity.GetCenterX() +
        entity.GetEdge().left * entity.GetScale()) &&
        (GetCenterX() + m_edge.left * GetScale() <= entity.GetCenterX() +
            entity.GetEdge().right * entity.GetScale()) &&
        (GetCenterY() + m_edge.bottom * GetScale() >= entity.GetCenterY() +
            entity.GetEdge().top* entity.GetScale()) &&
        (GetCenterY() + m_edge.top * GetScale() <= entity.GetCenterY() +
            entity.GetEdge().bottom * entity.GetScale())) {
        collisionVector = *entity.GetCenter() - *GetCenter();
        return true;
    }
    return false;

    return false;
}

bool CEntity::CollideRotatedBox(CEntity& entity, 
    VECTOR2& collisionVector) {
    // ȸ���� ���ڸ� �غ��Ѵ�.
    ComputeRotatedBox();        
    entity.ComputeRotatedBox();

    if (ProjectionsOverlap(entity) && entity.ProjectionsOverlap(*this)) {
        // �� ���ڸ� ���� ���������� �� ��ġ���� Ȯ���Ѵ�.
        collisionVector = *entity.GetCenter() - *GetCenter();
        return true;
    }
    return false;
}

void CEntity::ComputeRotatedBox() {
    if (m_rotatedBoxReady) {
        return;
    }
    float projection;

    // ���� ��ü�� ������ ����Ѵ�.
    VECTOR2 rotatedX(cos(m_spriteData.angle), sin(m_spriteData.angle));
    VECTOR2 rotatedY(-sin(m_spriteData.angle), cos(m_spriteData.angle));

    // ȸ���� ������ �𼭸��� ����Ѵ�.
    const VECTOR2* center = GetCenter();
    m_corner[0] = *center + rotatedX * ((float)m_edge.left * GetScale()) +
        rotatedY * ((float)m_edge.top * GetScale());
    m_corner[1] = *center + rotatedX * ((float)m_edge.right * GetScale()) +
        rotatedY * ((float)m_edge.top * GetScale());
    m_corner[2] = *center + rotatedX * ((float)m_edge.right * GetScale()) +
        rotatedY * ((float)m_edge.bottom * GetScale());
    m_corner[3] = *center + rotatedX * ((float)m_edge.left * GetScale()) +
        rotatedY * ((float)m_edge.bottom * GetScale());

    // �� ���� �������� �����Ѵ�.
    m_edge01 = VECTOR2(m_corner[1].x - m_corner[0].x, m_corner[1].y - m_corner[0].y);
    NSMath::Vector2Normalize(&m_edge01);
    m_edge03 = VECTOR2(m_corner[3].x - m_corner[0].x, m_corner[3].y - m_corner[0].y);
    NSMath::Vector2Normalize(&m_edge03);

    // ��ü�� �𼭸��� �����Ѵ�.
    // �� ��ü�� �𼭸��� ������ ����� �ּڰ��� �ִ��̴�.
    // edge01�� ����
    projection = NSMath::Vector2Dot(&m_edge01, &m_corner[0]);

    m_edge01Min = projection;
    m_edge01Max = projection;

    projection = NSMath::Vector2Dot(&m_edge01, &m_corner[1]);
    if (projection < m_edge01Min) {
        m_edge01Min = projection;
    }
    else if (projection > m_edge01Max) {
        m_edge01Max = projection;
    }

    // edge03�� ����
    projection = NSMath::Vector2Dot(&m_edge03, &m_corner[0]);

    m_edge03Min = projection;
    m_edge03Max = projection;

    projection = NSMath::Vector2Dot(&m_edge03, &m_corner[3]);
    if (projection < m_edge03Min) {
        m_edge03Min = projection;
    }
    else if (projection > m_edge03Max) {
        m_edge03Max = projection;
    }

    m_rotatedBoxReady = true;
}

bool CEntity::ProjectionsOverlap(CEntity& entity) {

}

bool CEntity::CollideRotatedBoxCircle(CEntity& entity, 
    VECTOR2& collisionVector)
{
    return false;
}

bool CEntity::CollideCornerCircle(VECTOR2 corner, 
    CEntity& entity, VECTOR2& collisionVector)
{
    return false;
}

CEntity::CEntity() {
    m_radius = 1.0f;
    
    m_edge.left = -1;
    m_edge.top = -1;
    m_edge.right = -1;
    m_edge.bottom = -1;

    m_mass = 1.0f;

    m_velocity.x = 0.0f;
    m_velocity.y = 0.0f;

    m_deltaVelocity.x = 0.0f;
    m_deltaVelocity.y = 0.0f;

    m_active = true;

    m_rotatedBoxReady = false;

    m_collisionType = NSEntity::COLLISION_TYPE::CIRCLE;

    m_health = 100;

    m_gravity = NSEntity::GRAVITY;
}

CEntity::~CEntity()
{
}

bool CEntity::Initialize(CGameEngine* pGame, int width, int height, int nCols, CTextureManager* textureManager) {
    m_input = pGame->GetInput();
    return (CImage::Initialize(pGame->GetGraphics(),
        width, height,
        nCols,
        textureManager));
}

void CEntity::Activation() {
    m_active = true;
}

void CEntity::Update(float frameTime) {
    // ���ν�Ƽ ���� ó���Ѵ�.
    m_velocity += m_deltaVelocity;

    m_deltaVelocity.x = 0;
    m_deltaVelocity.y = 0;

    CImage::Update(frameTime);

    m_rotatedBoxReady = false; // ȸ���� ������ �浹 ����.
}

void CEntity::Ai(float frameTime, CEntity& entity) {
}

bool CEntity::CollidesWith(CEntity& entity, VECTOR2& collisionVector) {
    if (!m_active || !entity.GetActive()) { // �� �� �ϳ��� ��Ȱ�� ���¶��
        return false;
    }

    // �� ��ü�� ��� CIRCLE�̶�� 
    if (m_collisionType == NSEntity::COLLISION_TYPE::CIRCLE &&
        entity.GetCollisionType() == NSEntity::COLLISION_TYPE::CIRCLE) {
        return CollideCircle(entity, collisionVector);
    }

    // �� ��ü�� ��� BOX��� 
    if (m_collisionType == NSEntity::COLLISION_TYPE::BOX &&
        entity.GetCollisionType() == NSEntity::COLLISION_TYPE::BOX) {
        return CollideBox(entity, collisionVector);
    }

    // ���⼭���ʹ� �ٸ� Ÿ���� ��ü �浹 Ÿ��
    // �� ��ü ���� �� ��Ŭ �浹 ����� ������� �ʴ´ٸ�
    if (m_collisionType != NSEntity::COLLISION_TYPE::CIRCLE &&
        entity.GetCollisionType() != NSEntity::COLLISION_TYPE::CIRCLE) {
        return CollideRotatedBox(entity, collisionVector);
    }
    else { // �� �� �ϳ��� ���� ���
        if (m_collisionType == NSEntity::COLLISION_TYPE::CIRCLE) {
            // �� ��ü�� �� �浹 ����� ����ϴ� ���
            return entity.CollideRotatedBoxCircle(*this, collisionVector);
        }
        else {
            // ��� ��ü�� �� �浹 ����� ����ϴ� ���
            CollideRotatedBoxCircle(entity, collisionVector);
        }
    }

    return false;
}

bool CEntity::OutsideRect(RECT rect) {
    // �� ��ü�� ������ �簢�� �ٱ��� �ִ°�
    if (m_spriteData.x + m_spriteData.width * GetScale() < rect.left ||
        m_spriteData.x > rect.right ||
        m_spriteData.y + m_spriteData.height * GetScale() < rect.top ||
        m_spriteData.y > rect.bottom) {
        return true;
    }
    return false;
}

// �� ��ü�� ���⿡ �¾� ���ظ� �Ծ���.
// ��ӹ޴� Ŭ���������� �� �Լ��� ������ �ؾ��Ѵ�.
void CEntity::Damage(int weapon) {
}
