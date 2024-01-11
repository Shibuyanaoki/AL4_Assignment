#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class Item  {

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, Vector3 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Updata();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform();

	void OnCollision();

	// getter
	//float GetRadius() { return radius_; }

	bool IsDead() const { return isDead_; }


private:

	WorldTransform worldTransform_;
	
	// モデル
	Model* model_ = nullptr;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 半径
	float radius_ = 2;

	// デスフラグ
	bool isDead_ = false;

};
