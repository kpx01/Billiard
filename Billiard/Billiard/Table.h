#pragma once

#include "Collider.h"

using namespace DirectX;
using namespace SimpleMath;

class Table : public Collider
{
	typedef Collider base;

	struct Wall
	{
		Vector3 normal;
		Vector3 pos;
		Wall() {};
		Wall(const Vector3 &n, const Vector3 &p) : normal(n), pos(p) {};
		~Wall() {};
	};

public:
	Table(Vector3, Vector3, float, float, bool);
	~Table();
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<ID3D11DeviceContext>);
	void Update(float elapsedTime);
	void Render(ComPtr<ID3D11DeviceContext>, XMMATRIX&, XMMATRIX&, XMMATRIX&);
	void Clear();

	void CalcCollisionTable(FLOAT, Vector3*, Vector3*, Vector3*, Vector3*, Vector3*, bool*, bool*, FLOAT*, Vector3*, bool*, Vector3*);

private:
	std::unique_ptr<CommonStates>		m_States = nullptr;
	std::unique_ptr<Model>				m_Model = nullptr;
	std::unique_ptr<EffectFactory>		m_FXFactory= nullptr;

	Wall	*m_Wall= (Wall*)malloc(sizeof(Wall));
	float	m_Time;
};

