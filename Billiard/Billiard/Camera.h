#pragma once

using namespace DirectX;
using namespace SimpleMath;

class Camera {
public:
	Camera();
	void Initialize();
	void Update();
	void SetCameraPosition(const Vector3&);
	void SetCameraLookAt(const Vector3&);
	void SetCameraUp(const Vector3&);
	Vector3 GetPosition() const;
	Vector3 GetLookAt() const;
	Vector3 GetUp() const;
	void Clear();

private:
	Vector3		m_CameraPosition;
	Vector3		m_CameraLookAt;
	Vector3		m_CameraUp;
	Vector3		distance = Vector3::Zero;
};