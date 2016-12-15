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
	direction.Normalize();

	if (GetAsyncKeyState(VK_RIGHT)) {
		direction.Cross(m_CameraUp, crossDirection);
		crossDirection.Normalize();
		m_CameraPosition -= crossDirection * 0.5f;
		m_CameraPosition += direction * 0.075f;
	}
	else if (GetAsyncKeyState(VK_LEFT)) {
		direction.Cross(m_CameraUp, crossDirection);
		crossDirection.Normalize();
		m_CameraPosition += crossDirection * 0.5f;
		m_CameraPosition += direction * 0.075f;
	}
	else if (GetAsyncKeyState(VK_UP)) {
		if (m_CameraPosition.y < Vector3::Distance(m_CameraPosition, m_CameraLookAt) * 0.8f) {
			direction.Cross(m_CameraUp, crossDirection);
			direction.Cross(-crossDirection, crossDirection);
			crossDirection.Normalize();
			m_CameraPosition += crossDirection * 0.5f;
			m_CameraPosition += direction * 0.075f;
		}
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		if (m_CameraPosition.y > 10.f) {
			direction.Cross(m_CameraUp, crossDirection);
			direction.Cross(-crossDirection, crossDirection);
			crossDirection.Normalize();
			m_CameraPosition -= crossDirection * 0.5f;
			m_CameraPosition += direction * 0.075f;
		}
	}

	if (Vector3::Distance(m_CameraPosition, m_CameraLookAt) >= 50.f)
		m_CameraPosition += direction * 0.1f;
	if (Vector3::Distance(m_CameraPosition, m_CameraLookAt) <= 50.f)
		m_CameraPosition -= direction * 0.1f;
}

void Camera::SetCameraPosition(const Vector3 &position) {
	m_CameraPosition = XMVectorSet(position.x, position.y, position.z, 0.f);
}

void Camera::SetCameraLookAt(const Vector3 &lookat) {
	m_CameraLookAt = XMVectorSet(lookat.x, lookat.y, lookat.z, 0.f);
}

void Camera::SetCameraUp(const Vector3 &up) {
	m_CameraUp = XMVectorSet(up.x, up.y, up.z, 0.f);
}

Vector3 Camera::GetPosition() const {
	return m_CameraPosition;
}

Vector3 Camera::GetLookAt() const {
	return m_CameraLookAt;
}

Vector3 Camera::GetUp() const {
	return m_CameraUp;
}

void Camera::Clear() {

}