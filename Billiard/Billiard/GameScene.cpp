#include "pch.h"
#include "GameScene.h"

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

GameScene::GameScene() :
	m_outputWidth(800),
	m_outputHeight(600)
{
}

void GameScene::Initialize(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext) {
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;

	m_Ball = std::make_unique<Ball>();
	m_Camera = std::make_unique<Camera>();

	m_States.reset(new CommonStates(m_d3dDevice.Get()));
	m_FXFactory.reset(new EffectFactory(m_d3dDevice.Get()));
	m_Batch.reset(new PrimitiveBatch<VertexPositionColor>(m_d3dContext.Get()));
	m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
	//m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"italic.spritefont"));
	m_BatchEffect.reset(new BasicEffect(m_d3dDevice.Get()));
	m_BatchEffect->SetVertexColorEnabled(true);
	
	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		HRESULT hr = m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&m_pBatchInputLayout);
		DX::ThrowIfFailed(hr);
	}
	
	m_World = XMMatrixIdentity();
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_outputWidth / (FLOAT)m_outputHeight, 0.01f, 1000.0f);

	m_BatchEffect->SetProjection(m_Projection);

	m_Model = Model::CreateFromCMO(m_d3dDevice.Get(), L"BilliardTable.cmo", *m_FXFactory, false);
	m_Ball->Initialize(m_d3dDevice, m_d3dContext);

	m_Camera->Initialize();
	m_Camera->SetCameraPosition(Vector3(0.0f, 30.0f, -50.0f));
	m_Camera->SetCameraLookAt(Vector3(0.0f, 0.0f, 0.0f));
	m_Camera->SetCameraUp(Vector3(0.0f, 1.0f, 0.0f));
	m_View = XMMatrixLookAtLH(Vector3(0.0f, 30.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	m_BatchEffect->SetView(m_View);
}

void GameScene::Update(float elapsedTime) {
	m_Camera->SetCameraLookAt(m_Ball->GetPosition());

	m_Ball->Update(elapsedTime);
	m_Camera->Update();
}

void GameScene::Draw() {

	XMVECTOR qid = XMQuaternionIdentity();
	const XMVECTORF32 scale = { 2.f, 2.f, 2.f };
	const XMVECTORF32 translate = { 0.f, 0.f, 0.f };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(0, 0, 0);
	XMMATRIX local = XMMatrixMultiply(m_World, XMMatrixTransformation(g_XMZero, qid, scale, g_XMZero, rotate, translate));
	m_Model->Draw(m_d3dContext.Get(), *m_States, local, m_Camera->GetView(), m_Projection);

	m_Ball->Render(m_World, m_Camera->GetView(), m_Projection);
}

void GameScene::Unload() {
	m_pBatchInputLayout.Reset();
	m_Ball->Clear();
	m_Camera->Clear();

	m_Camera.release();
	m_Ball.release();
}