#pragma once
#include "Table.h"
#include "Ball.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

class GameScene {
public :
	GameScene();
	~GameScene() {}

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

	std::unique_ptr<BasicEffect>					m_BatchEffect = nullptr;
	//std::unique_ptr<PrimitiveBatch<VertexPositionColor>>	m_Batch = nullptr;
	std::unique_ptr<SpriteBatch>					m_Sprites = nullptr;
	std::unique_ptr<SpriteFont>						m_Font = nullptr;
	std::unique_ptr<GeometricPrimitive>				m_Shape = nullptr;

	//XMMATRIX										m_World;
	//XMMATRIX										m_View;
	//XMMATRIX										m_Projection;

	Table											m_Table;
	Ball											m_Ball;
	Ball											*m_BallList = nullptr;
	Camera											m_Camera;

	Vector3 startpos[9] = { Vector3(0.f, 1.f, 25.f) ,Vector3(1.2f, 1.f, 27.2f) ,Vector3(-1.2f, 1.f, 27.2f) ,
							Vector3(2.4f, 1.f, 29.4f) ,Vector3(-2.4f, 1.f, 29.4f) ,Vector3(1.2f, 1.f, 31.6f) ,
							Vector3(-1.2f, 1.f, 31.6f) ,Vector3(0.f, 1.f, 33.8f) ,Vector3(0.f, 1.f, 29.4f) };

};