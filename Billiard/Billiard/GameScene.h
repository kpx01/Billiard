#pragma once
#include "Ball.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

__declspec(align(16)) class GameScene{
public :
	GameScene();
	~GameScene() {}

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	/* 初期化 */
	void Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	/* 更新 */
	void Update(float);
	/* 描画 */
	void Draw();
	/* ゲーム終了時処理 */
	void Unload();

private:
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_pBatchInputLayout = nullptr;

	std::unique_ptr<CommonStates>					m_States = nullptr;
	std::unique_ptr<BasicEffect>					m_BatchEffect = nullptr;
	std::unique_ptr<PrimitiveBatch<VertexPositionColor>>	m_Batch = nullptr;
	std::unique_ptr<Model>							m_Model = nullptr;
	std::unique_ptr<EffectFactory>					m_FXFactory = nullptr;
	std::unique_ptr<SpriteBatch>					m_Sprites = nullptr;
	//std::unique_ptr<SpriteFont>						m_Font = nullptr;

	XMMATRIX										m_World;
	XMMATRIX										m_View;
	XMMATRIX										m_Projection;

	std::unique_ptr<Ball>							m_Ball = nullptr;
	std::unique_ptr<Camera>							m_Camera = nullptr;
};