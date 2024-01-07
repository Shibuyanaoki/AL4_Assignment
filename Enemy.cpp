#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);


	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformL_arm_.translation_ = {0.9f, 1.0f, 0.0f};
	worldTransformR_arm_.translation_ = {-0.9f, 1.0f, 0.0f};

	// worldTransform_.UpdateMatrix();
}

void Enemy::Updata() {

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	Vector3 move = {0, 0, 0.5f};

    /* float speed = 0.5f;

	 move.z += speed;*/

	worldTransform_.rotation_.y += 0.5f;

	//move = Multiply(speed, Normalize(move));

	move = Transform(move, MakeRotateYmatrix(viewProjection_->rotation_.y));

	worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 体の部位を行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	ImGui::Begin("Enemy");
	float Body[3] = {
	    worldTransformBody_.translation_.x, worldTransformBody_.translation_.y,
	    worldTransformBody_.translation_.z};

	float ArmL[3] = {
	    worldTransformL_arm_.translation_.x, worldTransformL_arm_.translation_.y,
	    worldTransformL_arm_.translation_.z};

	float ArmR[3] = {
	    worldTransformR_arm_.translation_.x, worldTransformR_arm_.translation_.y,
	    worldTransformR_arm_.translation_.z};

	ImGui::SliderFloat3("Head Translation", Body, -30.0f, 30.0f);
	ImGui::SliderFloat3("ArmL Translation", ArmL, -10, 10);
	ImGui::SliderFloat3("ArmR Translation", ArmR, -30, 30);

	worldTransformBody_.translation_.x = Body[0];
	worldTransformBody_.translation_.y = Body[1];
	worldTransformBody_.translation_.z = Body[2];

	worldTransformL_arm_.translation_.x = ArmL[0];
	worldTransformL_arm_.translation_.y = ArmL[1];
	worldTransformL_arm_.translation_.z = ArmL[2];

	worldTransformR_arm_.translation_.x = ArmR[0];
	worldTransformR_arm_.translation_.y = ArmR[1];
	worldTransformR_arm_.translation_.z = ArmR[2];

	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// 敵の描画
	models_[kModeIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModeIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModeIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

const WorldTransform& Enemy::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します
	return worldTransform_;
}