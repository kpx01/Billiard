#pragma once

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

class SceneInterface {
public :
	/* ������ */
	virtual void Initialize(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext) {};
	/* �X�V */
	virtual void Update(float) {
	};
	/* �`�� */
	virtual void Draw() {};
	/* �Q�[���I�������� */
	virtual void Unload() {};
};