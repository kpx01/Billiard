//
// Object.cpp
//

#include "pch.h"
#include "Ball.h"

using namespace DirectX;
using namespace SimpleMath;

using Microsoft::WRL::ComPtr;

Ball::Ball() :
	m_pos(0.f, 20.f, 0.f),
	m_velocity(0.f, 0.f, 0.f),
	m_radius(1.5f),
	m_Time(0.f)
{
}

void Ball::Initialize(ComPtr<ID3D11Device> m_d3dDevice, ComPtr<ID3D11DeviceContext> m_d3dContext) {
	m_Shape = GeometricPrimitive::CreateSphere(m_d3dContext.Get(), m_radius, 16, false);
}

void Ball::Update(float elapsedTime) {
	m_Time += elapsedTime;

	// DirectXTK
	m_pos.x += m_velocity.x;
	m_pos.y += m_velocity.y;
	m_pos.z += m_velocity.z;
/*
	char str[100];
	sprintf(str, "%f\n", m_Time);
	OutputDebugStringA(str);
*/
}

void Ball::Render(XMMATRIX& m_World, XMMATRIX& m_View, XMMATRIX& m_Projection) {

	XMVECTOR qid = XMQuaternionIdentity();
	const XMVECTORF32 scale = { 1.f, 1.f, 1.f };
	const XMVECTORF32 translate = { m_pos.x, m_pos.y, m_pos.z };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(0, (XM_PI / 2.f) * m_Time, 0);
	XMMATRIX local = XMMatrixMultiply(m_World, XMMatrixTransformation(g_XMZero, qid, scale, g_XMZero, rotate, translate));

	// Draw 3D object
	//XMMATRIX local = XMMatrixMultiply(m_World, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
	m_Shape->Draw(local, m_View, m_Projection, Colors::Red, nullptr);//m_pTextureRV1.Get()

}

void Ball::Clear() {

}

void Ball::SetPosition(Vector3 position) {
	m_pos = position;
}

Vector3 Ball::GetPosition() {
	return m_pos;
}