#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Updata() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform();

private:

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	enum EnemyNum {
		kModeIndexBody,
		kModeIndexL_arm,
		kModeIndexR_arm,
	};
};
