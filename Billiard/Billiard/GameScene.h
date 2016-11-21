#pragma once
#include "SceneInterface.h"
#include "Ball.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

class GameScene : public SceneInterface {
public :
	GameScene();
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

	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_pBatchInputLayout;

	std::unique_ptr<CommonStates>					m_States;
	std::unique_ptr<BasicEffect>					m_BatchEffect;
	std::unique_ptr<PrimitiveBatch<VertexPositionColor>>	m_Batch;
	std::unique_ptr<Model>							m_Model;
	std::unique_ptr<EffectFactory>					m_FXFactory;
	std::unique_ptr<SpriteBatch>					m_Sprites;
	std::unique_ptr<SpriteFont>						m_Font;

	XMMATRIX										m_World;
	XMMATRIX										m_View;
	XMMATRIX										m_Projection;

	Ball											m_Ball;
	Camera											m_Camera;
};