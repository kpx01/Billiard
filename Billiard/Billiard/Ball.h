#pragma once

#include "Collider.h"
#include "Table.h"

using namespace DirectX;
using namespace SimpleMath;

class Ball : public Collider
{
	typedef Collider base;
public:
	Ball() {}
	Ball(const int&, const Vector3&, const Vector3&, const float&, const float&, const bool&);
	~Ball() {}
	void Initialize();
	void Initialize(const Vector3&);
	void Update(float elapsedTime);
	void Render(GeometricPrimitive*, XMMATRIX&, XMMATRIX&, XMMATRIX&);
	void Clear();
	float GetRadius() const;

	void CollisionTable(Table&);
	void CollisionBall(Ball&);

	bool CalcIntervalSphereSphere( float, const Ball&, const Ball&, float&, Vector3&, Vector3*, Vector3* );

private:
	int		m_id;
	float	m_radius;
	bool	m_fallBall;
	Vector3 m_fallHoll;

	//std::unique_ptr<GeometricPrimitive>	m_Shape;
};