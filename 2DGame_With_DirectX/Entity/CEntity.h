#pragma once

#include "../CGameEngine.h"
#include "../Graphics/CImage.h"
#include "../Math.h"
#include "../Audio/CAudio.h"

// �ݸ��� Ÿ���� �����Ѵ�.
namespace NSEntity {
	enum class COLLISION_TYPE {
		NONE,
		CIRCLE,
		BOX,
		ROTATED_BOX,
	};

	// ����� �ؼ� �ܿ� ã�� ��
	const FLOAT GRAVITY = 6.67428e-11f;
}

// �� Ŭ������ �ϴ°�
// 1. �ٸ� ��ü�� �浹�� �� �ִ� �ɷ�
// 2. �� ��ƼƼ�� �Ӽ��� ���� �ൿ ����
// 3. �̵� ��� ���� �ӷ� �� ��Ÿ �ӷ�
// 4. ���� �Ӽ��� �߷� �޼���
// 5. �ΰ����� ���� Shell �޼���
class CEntity : public CImage {
protected : 
	NSEntity::COLLISION_TYPE	m_collisionType;
	VECTOR2						m_center;
	VECTOR2						m_distSquared;
	FLOAT						m_sumRadiiSquared;
	FLOAT						m_radius;
	RECT						m_edge;
	VECTOR2						m_velocity;
	VECTOR2						m_deltaVelocity;
	FLOAT						m_mass;
	FLOAT						m_health;
	bool						m_active;
	bool						m_rotatedBoxReady;
	FLOAT						m_gravity;
	VECTOR2						m_corner[4];
	VECTOR2						m_edge01, m_edge03;  
	float						m_edge01Min, m_edge01Max, m_edge03Min, m_edge03Max;
	float						m_rr;
	float						m_force;

	CInput*						m_input;
	CAudio*						m_pAudio;

	virtual bool CollideCircle(CEntity& entity, VECTOR2& collisionVector);
	virtual bool CollideBox(CEntity& entity, VECTOR2& collisionVector);
	virtual bool CollideRotatedBox(CEntity& entity, VECTOR2& collisionVector);
	virtual void ComputeRotatedBox();
	virtual bool ProjectionsOverlap(CEntity& entity);
	// ȸ���� ���ڿ� ���� �浹����
	virtual bool CollideRotatedBoxCircle(CEntity& entity, VECTOR2& collisionVector);
	virtual bool CollideCornerCircle(VECTOR2 corner, CEntity& entity,
		VECTOR2& collisionVector);
	
public : 
	CEntity();
	virtual ~CEntity();

	virtual bool Initialize(CGameEngine* pGame, int width, int height,
		int nCols, CTextureManager* textureManager);
	virtual void Activation();

	virtual void Update(float frameTime);
	virtual void Ai(float frameTime, CEntity& entity);

	virtual bool CollidesWith(CEntity& entity, VECTOR2& collisionVector);
	virtual bool OutsideRect(RECT rect);

	virtual void Damage(int weapon);
	void Bounce(VECTOR2 collisionVector, CEntity& entity);
	void GravityForce(CEntity* entity, float frameTime);

	virtual NSEntity::COLLISION_TYPE GetCollisionType() const {
		return m_collisionType;
	}

	virtual void SetCollsionType(const NSEntity::COLLISION_TYPE& type) {
		m_collisionType = type;
	}

	virtual const VECTOR2* GetCenter() {
		m_center = VECTOR2(GetCenterX(), GetCenterY());
		return &m_center; 
	}

	virtual void SetCenter(const VECTOR2& vec) {
		this->m_center = vec;
	}

	virtual FLOAT GetRadius() const {
		return m_radius;
	}

	virtual void SetRadius(float radius) {
		m_radius = radius;
	}

	virtual RECT GetEdge() const {
		return m_edge;
	}

	virtual void SetEdge(const RECT& rect) {
		m_edge.left = rect.left;
		m_edge.right = rect.right;
		m_edge.top = rect.top;
		m_edge.bottom = rect.bottom;
	}

	virtual VECTOR2 GetVelocity() const {
		return m_velocity;
	}

	virtual void SetVelocity(const VECTOR2& velo) {
		m_velocity = velo;
	}

	virtual FLOAT GetMass() const {
		return m_mass;
	}

	virtual void SetMass(FLOAT mass) {
		m_mass = mass;
	}

	virtual FLOAT GetHealth() const {
		return m_health;
	}

	virtual VECTOR2* GetCorner(int idx) {
		return &m_corner[idx];
	}

	virtual void SetHealth(FLOAT health) {
		m_health = health;
	}

	virtual bool GetActive() const {
		return m_active;
	}

	virtual void SetActive(bool active) {
		m_active = active;
	}
};

