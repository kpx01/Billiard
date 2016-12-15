#include "pch.h"
#include "Table.h"

using namespace DirectX;
using namespace SimpleMath;

using Microsoft::WRL::ComPtr;

Table::Table(Vector3 pos, Vector3 velocity, float weight, float scale, bool freeze) :
	Collider(pos, velocity, weight, scale, freeze)
{
}

Table::~Table()
{
}

void Table::Initialize(ComPtr<ID3D11Device> m_d3dDevice, ComPtr<ID3D11DeviceContext> m_d3dContext) {
	m_States.reset(new CommonStates(m_d3dDevice.Get()));
	m_FXFactory.reset(new EffectFactory(m_d3dDevice.Get()));

	m_Model = Model::CreateFromCMO(m_d3dDevice.Get(), L"BilliardTable.cmo", *m_FXFactory, false);

	m_Wall = new Wall[5];
	m_Wall[0] = Wall(Vector3::Up, Vector3::Zero);
	m_Wall[1] = Wall(Vector3::Right, Vector3(-25.5f, 0.f, 0.f));
	m_Wall[2] = Wall(Vector3::Left, Vector3(25.5f, 0.f, 0.f));
	m_Wall[3] = Wall(Vector3::Backward, Vector3(0.f, 0.f, -51.f));
	m_Wall[4] = Wall(Vector3::Forward, Vector3(0.f, 0.f, 51.f));
}

void Table::Update(float elapsedTime)
{
	base::Update(elapsedTime);

	m_Time += elapsedTime;
}

void Table::Render(ComPtr<ID3D11DeviceContext> d3dContext, XMMATRIX& m_World, XMMATRIX& m_View, XMMATRIX& m_Projection) {

	XMVECTOR qid = XMQuaternionIdentity();
	const XMVECTORF32 scale = { m_scale , m_scale, m_scale };
	const XMVECTORF32 translate = { m_pos.x, m_pos.y - 4.f * 7.5f, m_pos.z };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(0, 0, 0);
	XMMATRIX local = XMMatrixMultiply(m_World, XMMatrixTransformation(g_XMZero, qid, scale, g_XMZero, rotate, translate));
	m_Model->Draw(d3dContext.Get(), *m_States, local, m_View, m_Projection);

}

void Table::Clear() {

}

// ボールとテーブルの衝突判定(当たっている壁の法線ベクトルを渡す)
void Table::CalcCollisionTable(
	FLOAT r,
	Vector3 *pPre_pos, Vector3 *pPos,
	Vector3 *preHitNormal, Vector3 *hitNormal, Vector3 *hitImpulseNormal,
	bool *hit, bool *hitImpulse,
	FLOAT *t,
	Vector3 *pOut_colli,
	bool *fallBall,
	Vector3 *fallholl
) {
	Vector3 holl[6] = { Vector3(27.5f, 0.f, 51.5f),  Vector3(-27.5f, 0.f, 51.5f),
						Vector3(27.5f, 0.f, 0.f),  Vector3(-27.f, 0.f, 0.f), 
						Vector3(27.5f, 0.f, -51.5f),  Vector3(-27.5f, 0.f, -51.5f)};

	Wall backWall[5] = { Wall(Vector3::Up, Vector3(0.f, -5.f, 0.f)),
						 Wall(Vector3::Right, Vector3(-3.f, 0.f, 0.f)),
						 Wall(Vector3::Left, Vector3(3.f, 0.f, 0.f)),
						 Wall(Vector3::Backward, Vector3(0.f, 0.f, -3.f)),
						 Wall(Vector3::Forward, Vector3(0.f, 0.f, 3.f))};

	for (int i = 0; i < 6; i++) {
		if (Vector3::Distance(*pPos, holl[i]) < 5.f) {
			*fallBall = true;
			*fallholl = holl[i];
		}
	}

	if (*fallBall) {
		for (int j = 0; j < 5; j++) {
			backWall[j].pos += *fallholl;
			if (CalcParticlePlaneCollision(r, pPre_pos, pPos, &backWall[j].normal, &backWall[j].pos, t, pOut_colli)) {
				*hit = true;
				*hitNormal += backWall[j].normal;
				if (*t > 0.f && *t < 1.f) {
					if (preHitNormal->x == 0 && preHitNormal->x != backWall[j].normal.x) {
						hitImpulseNormal->x = backWall[j].normal.x;
						*hitImpulse = true;
					}
					else if (preHitNormal->y == 0 && preHitNormal->y != backWall[j].normal.y) {
						hitImpulseNormal->y = backWall[j].normal.y;
						*hitImpulse = true;
					}
					else if (preHitNormal->z == 0 && preHitNormal->z != m_Wall[j].normal.z) {
						hitImpulseNormal->z = backWall[j].normal.z;
						*hitImpulse = true;
					}
					else
						*hitImpulse = false;
				}
			}
		}
	}
	else
	{
		for (int j = 0; j < 5; j++) {
			if (CalcParticlePlaneCollision(r, pPre_pos, pPos, &m_Wall[j].normal, &m_Wall[j].pos, t, pOut_colli)) {
				*hit = true;
				*hitNormal += m_Wall[j].normal;
				if (*t > 0.f && *t < 1.f) {
					if (preHitNormal->x == 0 && preHitNormal->x != m_Wall[j].normal.x) {
						hitImpulseNormal->x = m_Wall[j].normal.x;
						*hitImpulse = true;
					}
					else if (preHitNormal->y == 0 && preHitNormal->y != m_Wall[j].normal.y) {
						hitImpulseNormal->y = m_Wall[j].normal.y;
						*hitImpulse = true;
					}
					else if (preHitNormal->z == 0 && preHitNormal->z != m_Wall[j].normal.z) {
						hitImpulseNormal->z = m_Wall[j].normal.z;
						*hitImpulse = true;
					}
					else
						*hitImpulse = false;
				}
			}
		}
	}
}