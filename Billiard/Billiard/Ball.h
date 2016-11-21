#pragma once

using namespace DirectX;
using namespace SimpleMath;

__declspec(align(16)) class Ball
{
public:
	Ball();
	~Ball() {}

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<ID3D11DeviceContext>);
	void Update(float elapsedTime);
	void Render(XMMATRIX&, XMMATRIX&, XMMATRIX&);
	void Clear();
	void SetPosition(Vector3);
	Vector3 GetPosition();

private:
	std::unique_ptr<GeometricPrimitive>	m_Shape = nullptr;

	Vector3	m_pos;
	Vector3	m_velocity;
	
	float	m_radius;
	float	m_Time;
};