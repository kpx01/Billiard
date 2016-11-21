//
//	Camera.h
//
#pragma once

using namespace DirectX;
using namespace SimpleMath;

__declspec(align(16)) class Camera {
public:
	Camera();
	~Camera(){}

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

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