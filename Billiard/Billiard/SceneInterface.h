#pragma once

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

class SceneInterface {
public :
	/* 初期化 */
	virtual void Initialize(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext) {};
	/* 更新 */
	virtual void Update(float) {
	};
	/* 描画 */
	virtual void Draw() {};
	/* ゲーム終了時処理 */
	virtual void Unload() {};
};