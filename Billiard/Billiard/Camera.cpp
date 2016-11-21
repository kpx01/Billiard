#include "pch.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

using Microsoft::WRL::ComPtr;

Camera::Camera():
	m_CameraPosition(0.f, 0.f, 0.f),
	m_CameraLookAt(0.f, 0.f, 0.f),
	m_CameraUp(0.f, 0.f, 0.f)
{

}

void Camera::Initialize() {
}

void Camera::Update() {
	Vector3 direction;
	Vector3 crossDirection;
	direction = m_CameraLookAt - m_CameraPosition;

	if (GetAsyncKeyState(VK_RIGHT)) {
		direction.Cross(m_CameraUp, crossDirection);
		crossDirection.Normalize();
		m_CameraPosition -= crossDirection;
	}
	else if (GetAsyncKeyState(VK_LEFT)) {
		direction.Cross(m_CameraUp, crossDirection);
		crossDirection.Normalize();
		m_CameraPosition += crossDirection;
	}
	else if (GetAsyncKeyState(VK_UP)) {
		//direction.Cross(m_CameraUp.Left, crossDirection);
		//crossDirection.Normalize();
		//m_CameraPosition += crossDirection;
		direction.Normalize();
		m_CameraPosition += direction;
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		//direction.Cross(m_CameraUp.Left, crossDirection);
		//crossDirection.Normalize();
		//m_CameraPosition -= crossDirection;
		direction.Normalize();
		m_CameraPosition -= direction;
	}
}

void Camera::SetCameraPosition(Vector3 position) {
	m_CameraPosition = XMVectorSet(position.x, position.y, position.z, 0.f);
}

void Camera::SetCameraLookAt(Vector3 lookat) {
	m_CameraLookAt = XMVectorSet(lookat.x, lookat.y, lookat.z, 0.f);
}

void Camera::SetCameraUp(Vector3 up) {
	m_CameraUp = XMVectorSet(up.x, up.y, up.z, 0.f);
}

XMMATRIX Camera::GetView() {
	m_View = XMMatrixLookAtLH(m_CameraPosition, m_CameraLookAt, m_CameraUp);
	return m_View;
}

void Camera::Clear() {

}