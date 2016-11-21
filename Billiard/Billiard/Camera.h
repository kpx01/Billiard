#pragma once

using namespace DirectX;
using namespace SimpleMath;

class Camera {
public:
	Camera();
	void Initialize();
	void Update();
	void SetCameraPosition(Vector3);
	void SetCameraLookAt(Vector3);
	void SetCameraUp(Vector3);
	XMMATRIX GetView();
	void Clear();

private:
	Vector3		m_CameraPosition;
	Vector3		m_CameraLookAt;
	Vector3		m_CameraUp;

	XMMATRIX	m_View;
};