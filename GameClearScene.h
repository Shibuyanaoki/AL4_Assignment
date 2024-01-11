#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Scene.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class GameClearScene {
public:
	void Initialize();

	void Updata();

	void Draw();

	bool IsSceneEnd() { return isSceneEnd; }

	Scene NextScene() { return Scene::TITLE; }

	/// <summary>
	/// スコアの描画処理
	/// </summary>
	void GamePlayDraw2DNear();

	/// <summary>
	/// スコアの計算と描画処理
	/// </summary>
	void DrawScore();

	// セッター
	int SetScore(int gameScore) { return gameScore_ = gameScore; };

	void Reset();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビューポートプロジェクション
	ViewProjection viewProjection_;

	// シーンを終わらせるフラグ
	bool isSceneEnd = false;

	// ナンバー
	uint32_t textureHandleNumber = 0;
	Sprite* spriteNumber_[4] = {};

	// スコア
	uint32_t textureHandleSCORE = 0;
	Sprite* spriteScore = {};
	int gameScore_ = 0;

	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;
};
