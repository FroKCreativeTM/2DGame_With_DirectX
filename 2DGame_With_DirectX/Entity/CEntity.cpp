#include "CEntity.h"

// 원의 거리 방정식을 이용해서 두 원의 충돌을 검증한다.
bool CEntity::CollideCircle(CEntity& entity, 
    VECTOR2& collisionVector)
{
    // 두 중심 사이의 차이를 구한다.
    m_distSquared = *GetCenter() - *entity.GetCenter();
    m_distSquared.x = m_distSquared.x * m_distSquared.x;
    m_distSquared.y = m_distSquared.y * m_distSquared.y;

    // 각 원의 반지름의 합을 구한다.
    m_sumRadiiSquared = (m_radius * GetScale()) +
        (entity.m_radius * entity.GetScale());
    m_sumRadiiSquared *= m_sumRadiiSquared;

    if (m_distSquared.x + m_distSquared.y <= m_sumRadiiSquared) {
        // 개체들이 충돌한 경우
        collisionVector = *entity.GetCenter() - *GetCenter(); // 충돌한 곳의 벡터값
        return true;
    }
    return false; // 충돌하지 않음.
}

// A의 오른쪽 모서리 >= B의 왼쪽 모서리
// A의 왼쪽 모서리 <= B의 오른쪽 모서리
// A의 아랫쪽 모서리 >= B의 위쪽 모서리
// A의 위쪽 모서리 <= B의 아래쪽 모서리
bool CEntity::CollideBox(CEntity& entity, 
    VECTOR2& collisionVector) {
    if (!m_active || !entity.GetActive()) { // 둘 중 하나가 활성화가 안 되어있다면
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
}

bool CEntity::CollideRotatedBox(CEntity& entity, 
    VECTOR2& collisionVector) {
    // 회전된 상자를 준비한다.
    ComputeRotatedBox();        
    entity.ComputeRotatedBox();

    if (ProjectionsOverlap(entity) && entity.ProjectionsOverlap(*this)) {
        // 두 상자를 선에 투영시켰을 때 겹치는지 확인한다.
        collisionVector = *entity.GetCenter() - *GetCenter();
        return true;
    }
    return false;
}

// 회전된 상자의 모서리와 투영된 모서리, 그리고 투영선의 최솟값 및 최댓값을 계산한다.
// 0--1 모서리 인덱스
// |  |
// 3--2
void CEntity::ComputeRotatedBox() {
    if (m_rotatedBoxReady) {
        return;
    }
    float projection;

    // 현재 개체의 각도를 계산한다.
    VECTOR2 rotatedX(cos(m_spriteData.angle), sin(m_spriteData.angle));
    VECTOR2 rotatedY(-sin(m_spriteData.angle), cos(m_spriteData.angle));

    // 회전된 상자의 모서리를 계산한다.
    const VECTOR2* center = GetCenter();
    m_corner[0] = *center + rotatedX * ((float)m_edge.left * GetScale()) +
        rotatedY * ((float)m_edge.top * GetScale());
    m_corner[1] = *center + rotatedX * ((float)m_edge.right * GetScale()) +
        rotatedY * ((float)m_edge.top * GetScale());
    m_corner[2] = *center + rotatedX * ((float)m_edge.right * GetScale()) +
        rotatedY * ((float)m_edge.bottom * GetScale());
    m_corner[3] = *center + rotatedX * ((float)m_edge.left * GetScale()) +
        rotatedY * ((float)m_edge.bottom * GetScale());

    // 두 개의 투영선을 생성한다.
    m_edge01 = VECTOR2(m_corner[1].x - m_corner[0].x, m_corner[1].y - m_corner[0].y);
    NSMath::Vector2Normalize(&m_edge01);
    m_edge03 = VECTOR2(m_corner[3].x - m_corner[0].x, m_corner[3].y - m_corner[0].y);
    NSMath::Vector2Normalize(&m_edge03);

    // 개체를 모서리에 투영한다.
    // 이 개체는 모서리로 투영한 결과의 최솟값과 최댓값이다.
    // edge01에 투영
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

    // edge03에 투영
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

// 다른 상자를 edge01과 edge03에 투영한다.
bool CEntity::ProjectionsOverlap(CEntity& entity) {
    // 투영을 위한 변수들
    float projection, min01, max01, min03, max03;

    // 먼저 투영값을 구한다.
    projection = NSMath::Vector2Dot(&m_edge01, entity.GetCorner(0));

    // 모서리 0에 투영한다.
    min01 = max01 = projection;

    // 각 코너마다 내적을 구해 투영한다.
    for (int c = 0; c < 4; c++) {
        projection = NSMath::Vector2Dot(&m_edge01, entity.GetCorner(c));

        if (projection < min01) {
            min01 = projection;
        }
        else if (projection > max01) {
            max01 = projection;
        }
    }

    // 만약 투영선에 걸치지 않는다면
    if (min01 > m_edge01Max || max01 < m_edge01Min) {
        return false; // 충돌이 일어나지 않은 것이다.
    }


    // 03부분
    projection = NSMath::Vector2Dot(&m_edge03, entity.GetCorner(0));

    // 모서리 0에 투영한다.
    min03 = max03 = projection;

    // 각 코너마다 내적을 구해 투영한다.
    for (int c = 0; c < 4; c++) {
        projection = NSMath::Vector2Dot(&m_edge03, entity.GetCorner(c));

        if (projection < min03) {
            min03 = projection;
        }
        else if (projection > max03) {
            max03 = projection;
        }
    }

    // 만약 투영선에 걸치지 않는다면
    if (min03 > m_edge03Max || max03 < m_edge03Min) {
        return false; // 충돌이 일어나지 않은 것이다.
    }

    // 충돌된 것이다.
    return true;
}

// 회전된 상자와 원의 충돌을 검사하는 함수이다.
// Collision에서 실행한다.
// 상자의 모서리와 원의 반지름에 대해 분리된 축 검사를 사용한다.
// 원의 중심이 충돌 상자의 모서리를 연장한 선보다 바깥에 있다면,
// (보로노이의 영역이라고 한다.) 거리 검사를 사용해서
// 가장 가까운 상자의 모서리에 대해 충돌 검사를 수행한다.
// 가장 가까운 모서리는 겹침 검사로부터 결정된다.
// 보로노이0   |         | 보로노이1
//     ------ 0 ------- 1 ----------
//            |         |
//     ------ 3 ------- 2 ----------
// 보로노이3   |         | 보로노이2
// 이후 : 충돌하는 경우 true, 충돌하지 않는 경우 false,
// 충돌 뒤 collsionVector를 설정한다.
bool CEntity::CollideRotatedBoxCircle(CEntity& entity, 
    VECTOR2& collisionVector) {
    float min01, max01, min03, max03, center01, center03;
    // 회전된 상자 준비
    ComputeRotatedBox();

    // 원의 중심을 edge01에 투영한다.
    center01 = NSMath::Vector2Dot(&m_edge01, entity.GetCenter());
    min01 = center01 - entity.GetRadius() * entity.GetScale();
    max01 = center01 + entity.GetRadius() * entity.GetScale();

    // 투영선에 겹치지 않는다면
    if (min01 > m_edge01Max || max01 < m_edge01Min) {
        return false;
    }

    center03 = NSMath::Vector2Dot(&m_edge03, entity.GetCenter());
    min03 = center03 - entity.GetRadius() * entity.GetScale();
    max03 = center03 + entity.GetRadius() * entity.GetScale();

    // 투영선에 겹치지 않는다면
    if (min03 > m_edge03Max || max03 < m_edge03Min) {
        return false;
    }

    // 원에 대한 투영선과 상자에 대한 투영선을 겹친다.
    // 원이 충돌 상자의 보로노이 영역 안에 있는지 확인한다.
    if (center01 < m_edge01Min && center03 < m_edge03Min) { // 보로노이0
        return CollideCornerCircle(m_corner[0], entity, collisionVector);
    }
    if (center01 > m_edge01Max && center03 < m_edge03Min) { // 보로노이1
        return CollideCornerCircle(m_corner[1], entity, collisionVector);
    }
    if (center01 > m_edge01Max && center03 > m_edge03Max) { // 보로노이2
        return CollideCornerCircle(m_corner[2], entity, collisionVector);
    }
    if (center01 < m_edge01Min && center03 > m_edge03Max) { // 보로노이3
        return CollideCornerCircle(m_corner[3], entity, collisionVector);
    }

    // 원이 보로노이 영역에 없다면 상자의 모서리와 충돌했음을 의미
    // 충돌 벡터를 설정하고, 원의 중심을 상자의 중심으로 사용
    collisionVector = *entity.GetCenter() - *GetCenter();

    return true;
}

// 원이 보로노이 영역에 있다면 처리할 수 있게 만들어주는 함수이다.
// 거리 검사를 사용해서 상자 모서리와 원의 충돌을 검사한다.
// CollideRotatedBoxCircle()에서 호출한다.
// 이후 : 출동하는 경우 true, 아닌 경우 false,
// 충돌한 경우 collisionVector를 설정한다.
bool CEntity::CollideCornerCircle(VECTOR2 corner, 
    CEntity& entity, VECTOR2& collisionVector) {
    m_distSquared = corner - *entity.GetCenter(); // 중심 - 원

    m_distSquared.x = m_distSquared.x * m_distSquared.x; // 제곱 차이
    m_distSquared.y = m_distSquared.y * m_distSquared.y;

    // 각 원의 반지름의 합을 구한다.
    m_sumRadiiSquared = (m_radius * GetScale()) + (entity.m_radius * entity.GetScale());
    m_sumRadiiSquared *= m_sumRadiiSquared;

    if (m_distSquared.x + m_distSquared.y <= m_sumRadiiSquared) {
        // 충돌 벡터를 설정한다.
        collisionVector = *entity.GetCenter() - corner;
        return true;
    }
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

CEntity::~CEntity() {
}

bool CEntity::Initialize(CGameEngine* pGame, 
    int width, int height, int nCols, 
    CTextureManager* textureManager) {
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
    // 벨로시티 값을 처리한다.
    m_velocity += m_deltaVelocity;

    m_deltaVelocity.x = 0;
    m_deltaVelocity.y = 0;

    CImage::Update(frameTime);

    m_rotatedBoxReady = false; // 회전된 상자의 충돌 감지.
}

void CEntity::Ai(float frameTime, CEntity& entity) {
}

bool CEntity::CollidesWith(CEntity& entity, VECTOR2& collisionVector) {
    if (!m_active || !entity.GetActive()) { // 둘 중 하나라도 비활성 상태라면
        return false;
    }

    // 두 개체가 모두 CIRCLE이라면 
    if (m_collisionType == NSEntity::COLLISION_TYPE::CIRCLE &&
        entity.GetCollisionType() == NSEntity::COLLISION_TYPE::CIRCLE) {
        return CollideCircle(entity, collisionVector);
    }

    // 두 개체가 모두 BOX라면 
    if (m_collisionType == NSEntity::COLLISION_TYPE::BOX &&
        entity.GetCollisionType() == NSEntity::COLLISION_TYPE::BOX) {
        return CollideBox(entity, collisionVector);
    }

    // 여기서부터는 다른 타입의 객체 충돌 타입
    // 두 개체 전부 다 서클 충돌 방식을 사용하지 않는다면
    if (m_collisionType != NSEntity::COLLISION_TYPE::CIRCLE &&
        entity.GetCollisionType() != NSEntity::COLLISION_TYPE::CIRCLE) {
        return CollideRotatedBox(entity, collisionVector);
    }
    else { // 둘 중 하나가 원인 경우
        if (m_collisionType == NSEntity::COLLISION_TYPE::CIRCLE) {
            // 이 객체가 원 충돌 방식을 사용하는 경우
            return entity.CollideRotatedBoxCircle(*this, collisionVector);
        }
        else {
            // 상대 객체가 원 충돌 방식을 사용하는 경우
            CollideRotatedBoxCircle(entity, collisionVector);
        }
    }

    return false;
}

bool CEntity::OutsideRect(RECT rect) {
    // 이 개체가 지정된 사각형 바깥에 있는가
    if (m_spriteData.x + m_spriteData.width * GetScale() < rect.left ||
        m_spriteData.x > rect.right ||
        m_spriteData.y + m_spriteData.height * GetScale() < rect.top ||
        m_spriteData.y > rect.bottom) {
        return true;
    }
    return false;
}

// 이 개체는 무기에 맞아 피해를 입었다.
// 상속받는 클래스에서는 이 함수를 재정의 해야한다.
void CEntity::Damage(int weapon) {
}
