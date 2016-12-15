#include "pch.h"
#include "GameScene.h"

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

GameScene::GameScene() :
	m_outputWidth(800),
	m_outputHeight(600),
	m_Ball(0, Vector3(0.f, 1.f, -25.f), Vector3(0.f, 0.f, 0.f), 1.f, 1.f, false),
	m_Table(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f), 1.f, 4.f, true)
{
}

void GameScene::Initialize(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext) {
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;

	sprintf(m_Message, "Press Space Button");

	m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
	m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"italic.spritefont"));
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

	m_BatchEffect->SetProjection(XMMatrixPerspectiveFovLH(XM_PIDIV4, m_outputWidth / (FLOAT)m_outputHeight, 0.01f, 1000.0f));

	m_Table.Initialize(m_d3dDevice, m_d3dContext);
	m_Ball.Initialize();

	m_BallList = new Ball[9];
	for (int i = 0; i < 9; i++) {
		m_BallList[i] = Ball(i + 1, startpos[i], Vector3(0.f, 0.f, 0.f), 1.f, 1.f, false);
		m_BallList[i].Initialize();
	}

	m_Camera.Initialize();
	m_Camera.SetCameraPosition(Vector3(0.0f, 25.0f, -70.0f));
	m_Camera.SetCameraLookAt(Vector3(0.0f, 0.0f, 0.0f));
	m_Camera.SetCameraUp(Vector3(0.0f, 1.0f, 0.0f));
	m_BatchEffect->SetView(XMMatrixLookAtLH(m_Camera.GetPosition(), m_Camera.GetLookAt(), m_Camera.GetUp()));
}

void GameScene::Update(float elapsedTime) {
	
	m_Camera.SetCameraLookAt(m_Ball.GetPosition());

	Vector3 direction = m_Ball.GetPosition() - m_Camera.GetPosition();
	Vector3 crossDirection;
	direction.y = 0.f;
	direction.Normalize();
	direction.Cross(m_Camera.GetUp(), crossDirection);
	crossDirection.Normalize();

	// デバッグ用の入力WASDで上下左右に動く
	if (GetAsyncKeyState('W')) 
		m_Ball.AddForce(direction * 100.f);
	if (GetAsyncKeyState('A')) 
		m_Ball.AddForce(crossDirection * 100.f);
	if (GetAsyncKeyState('S')) 
		m_Ball.AddForce(direction * -100.f);
	if (GetAsyncKeyState('D')) 
		m_Ball.AddForce(crossDirection * -100.f);

	// カメラの方向へ打つ
	if (GetAsyncKeyState(VK_SPACE) && m_Ball.GetVelocity() == Vector3::Zero)
		m_Ball.AddForce(direction * 7000.f);
	// 初期位置に戻す
	if (GetAsyncKeyState(VK_ESCAPE))
		m_Ball.Initialize(Vector3(0.f, 1.f, -25.f));

	// オブジェクトの更新, 衝突判定
	m_Table.Update(elapsedTime);
	m_Ball.Update(elapsedTime);

	for (int i = 0; i < 9; i++) {
		m_BallList[i].Update(elapsedTime);
		m_BallList[i].CollisionTable(m_Table);
		m_BallList[i].CollisionBall(m_Ball);
		m_Ball.CollisionBall(m_BallList[i]);
		for (int j = i; j < 9; j++) {
			if (i == j) continue;
			m_BallList[i].CollisionBall(m_BallList[j]);
			m_BallList[j].CollisionBall(m_BallList[i]);
		}
	}

	m_Ball.CollisionTable(m_Table);
	m_Camera.Update();

	// 落ちたボールの判定,表示
	if (m_Ball.GetFallBall()) {
		m_FallBallTime += elapsedTime;
		if (m_FallBallTime > 2.f) {
			m_Ball.Initialize(Vector3(0.f, 1.f, -25.f));
			m_FallBallTime = 0.f;
		}
	}

	for (int i = 0; i < 9; i++) {
		if (m_bBallFinished[i] || m_bGameFinish) continue;
		m_bBallFinished[i] = m_BallList[i].GetFallBall();
		if (m_BallList[i].GetFallBall()) {
			if (i == 8) {
				sprintf(m_Message, "Cup in No.%d\nFinished!!", i + 1);
				m_bGameFinish = true;
			}
			else
				sprintf(m_Message, "Cup in No.%d", i + 1);
		}
	}

}

void GameScene::Draw() {
	m_Shape = GeometricPrimitive::CreateSphere(m_d3dContext.Get(), 2.f, 16, false);
	m_Table.Render(m_d3dContext, XMMatrixIdentity(), XMMatrixLookAtLH(m_Camera.GetPosition(), m_Camera.GetLookAt(), m_Camera.GetUp()), XMMatrixPerspectiveFovLH(XM_PIDIV4, m_outputWidth / (FLOAT)m_outputHeight, 0.01f, 1000.0f));
	m_Ball.Render(m_Shape.get(), XMMatrixIdentity(), XMMatrixLookAtLH(m_Camera.GetPosition(), m_Camera.GetLookAt(), m_Camera.GetUp()), XMMatrixPerspectiveFovLH(XM_PIDIV4, m_outputWidth / (FLOAT)m_outputHeight, 0.01f, 1000.0f));

	for (int i = 0; i < 9; i++) 
		m_BallList[i].Render(m_Shape.get(), XMMatrixIdentity(), XMMatrixLookAtLH(m_Camera.GetPosition(), m_Camera.GetLookAt(), m_Camera.GetUp()), XMMatrixPerspectiveFovLH(XM_PIDIV4, m_outputWidth / (FLOAT)m_outputHeight, 0.01f, 1000.0f));

	m_Sprites->Begin(SpriteSortMode_Deferred);
	wchar_t str[100];
	mbstowcs(str, m_Message, 100);
	m_Font->DrawString(m_Sprites.get(), str, XMFLOAT2(10, 10), Colors::White);
	m_Sprites->End();
}

void GameScene::Unload() {
	m_pBatchInputLayout.Reset();
	m_Table.Clear();
	m_Ball.Clear();
	for (int i = 0; i < 9; i++)
		m_BallList[i].Clear();
	delete m_BallList;
	m_Camera.Clear();
}