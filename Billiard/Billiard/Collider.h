#pragma once

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

class Collider
{
public:
	Collider() {}
	Collider(Vector3, Vector3, float, float, bool);
	~Collider();
	void Update(float);
	void SetPosition(const Vector3&);
	Vector3 GetPosition() const;
	Vector3 GetVelocity() const;
	float GetWeight() const;
	float GetRestitution() const;

	void AddForce(Vector3);
	bool CalcParticlePlaneCollision(FLOAT, Vector3*, Vector3*, Vector3*, Vector3*, FLOAT*, Vector3*);
	bool CalcParticleColliAfterPos(
		Vector3*, Vector3*,
		Vector3*, Vector3*,
		FLOAT, FLOAT,
		FLOAT, FLOAT,
		FLOAT,
		Vector3*, Vector3*,
		Vector3*, Vector3*
	);

protected :
	Vector3	m_prepos;
	Vector3	m_pos;
	Vector3	m_velocity;
	Vector3 m_force;
	Vector3 m_gravity;
	float	m_time;
	float	m_scale;
	float	m_weight;
	float	m_restitution;
	bool	m_freeze;

	bool	m_hit;
	bool	m_hitImpulse;
	bool	m_hitBall;
	Vector3 m_hitNormal;
	Vector3	m_hitImpulseNormal;
	Vector3 m_preHitNormal;
	Vector3 m_hitPoint;
	float	m_hitTime;

	float	m_hitBallTime;
	Vector3 m_hitBallPosition;
	Vector3 m_outHitPosition;
	Vector3 m_hitBallVelocity;
	float	m_hitBallWeight;
	float	m_hitBallRestitution;
};

