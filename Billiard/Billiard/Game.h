//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "GameScene.h"

using namespace DirectX;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();
	void Present();

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	// Device resources.
	HWND                                            m_window = nullptr;
	int                                             m_outputWidth = 800;
	int                                             m_outputHeight = 600;

	D3D_FEATURE_LEVEL                               m_featureLevel = D3D_FEATURE_LEVEL_11_1;
	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext = nullptr;

	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1 = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDepthStencil = nullptr;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	std::unique_ptr<GameScene>						m_Scene = nullptr;
};