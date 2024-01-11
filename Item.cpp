#include "Item.h"
#include "ImGuiManager.h"

void Item::Initialize(Model* model, Vector3 position) {

	// NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;
	// ポジション
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {1, 1, 1};

	// ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.UpdateMatrix();
}

void Item::Updata() { worldTransform_.UpdateMatrix(); }

void Item::Draw(const ViewProjection& viewProjection) {
	// 敵の描画
	 model_->Draw(worldTransform_, viewProjection); 
}

Vector3 Item::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

const WorldTransform& Item::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します
	return worldTransform_;
}

void Item::OnCollision() {

	isDead_ = true;



}
