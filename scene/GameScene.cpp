#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビューポートプロジェクションの初期化
	viewProjection_.Initialize();

	// 自機の体の3Dモデルの生成
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	// 自機の頭の3Dモデルの生成
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	// 自機の左腕の3Dモデルの生成
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	// 自機の右腕の3Dモデルの生成
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));

	// 敵の体の3Dモデルの生成
	// modelEnemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	//// 敵の左の3Dモデルの生成
	// modelEnemyL_arm.reset(Model::CreateFromOBJ("needle_L_arm", true));
	//// 敵の左右の3Dモデルの生成
	// modelEnemyR_arm.reset(Model::CreateFromOBJ("needle_R_arm", true));

	// アイテムの3Dモデルの生成
	modelItem_.reset(Model::CreateFromOBJ("Insect", true));

	// ハンマーの3Dモデルの生成
	modelHammer_.reset(Model::CreateFromOBJ("hammer", true));

	// 天球の3Dモデル生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// 地面の3Dモデル生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));

	// 自キャラモデルまとめ
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(), modelHammer_.get()};

	// 自キャラの生成と初期化処理
	player_ = std::make_unique<Player>();

	player_->Initialize(playerModels);

	// 天球の生成と初期化処理
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	// 地面の生成と初期化処理
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	// 追従カメラの生成と初期化処理
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラのセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// followCamera_->SetTarget(&enemy_->GetWorldTransform());

	// 自キャラのビュープロジェクションに追従カメラのビュープロジェクションをセット
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// 　敵のモデルまとめ
	/*std::vector<Model*> ItemModels = {
	    modelEnemyBody_.get(), modelEnemyL_arm.get(), modelEnemyR_arm.get()};*/

	//// 敵の生成と初期化処理
	// Item_ = std::make_unique<Item>();
	// Item_->Initialize(ItemModels);

	// Item_->SetViewProjection(&followCamera_->GetViewProjectionEnemy());

	// CSVファイル読み込み
	LoadPointPopData();
}

void GameScene::Update() {

	// 自キャラの更新
	player_->Updata();

	//// 敵の更新
	// Item_->Updata();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	// 追従カメラの更新
	followCamera_->Update();

	CheckAllCollision();

	// ビュープロジェクションの反映
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();

	items_.remove_if([](std::unique_ptr<Item>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	// CSVファイルの更新処理
	UpdataPointPopCommands();

	if (itemCount >= 5) {
		isSceneEnd = true;
		itemCount = 0;
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// プレイヤーの描画
	player_->Draw(viewProjection_);

	for (const std::unique_ptr<Item>& item : items_) {
		item->Draw(viewProjection_);
	}

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// 地面の描画
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {

	Vector3 posA, posB;

	if (player_->HammerFlag() == true) {

		for (const std::unique_ptr<Item>& item : items_) {
			posA = player_->GetHammerWorldPosition();

			posB = item->GetWorldPosition();

			float Hit = (posA.x - posB.x) * (posA.x - posB.x) +
			            (posA.y - posB.y) * (posA.y - posB.y) +
			            (posA.z - posB.z) * (posA.z - posB.z);

			float Radius = (player_->GetRadius() + 2 /*Item_->GetRadius()*/) *
			               (player_->GetRadius() + 2 /*Item_->GetRadius()*/);

			if (Hit <= Radius) {
				item->OnCollision();
				itemCount++;
			}
		}
	}
}

void GameScene::LoadPointPopData() {
	pointPopCommnds.clear();
	std::ifstream file;
	file.open("Resources/ItemPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	pointPopCommnds << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdataPointPopCommands() {
	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(pointPopCommnds, line)) {
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を所得

		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			PointGenerate({x, y, z});
		}
	}
}

void GameScene::PointGenerate(Vector3 position) {
	//  アイテムの生成と初期化処理
	Item* item = new Item();
	item->Initialize(modelItem_.get(), position);

	items_.push_back(static_cast<std::unique_ptr<Item>>(item));
}

void GameScene::Reset() {
	isSceneEnd = false;

	player_->ResetPosition();

	followCamera_->ResetPosition();

	for (const std::unique_ptr<Item>& item : items_) {
		item->OnCollision();
	}

	// CSVファイル読み込み
	LoadPointPopData();
}
