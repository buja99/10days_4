#include "GameScene.h"
#include "SceneManager.h"
#ifdef _DEBUG
#include "ImGuiManager.h"
#endif // _DEBUG
#include "MyMath.h"
#include "LevelObjectBuilder.h"
#include "PostProcessManager.h"
#include "GrayscaleEffect.h"
#include "VignetteEffect.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "Object3dCommon.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Object3d.h"

GameScene::~GameScene() {
}



void GameScene::Initialize() {

	audio_ = Sound::GetInstance();
	input_ = Input::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	camera_ = std::make_unique<Camera>();

	TextureManager::GetInstance()->LoadTexture("resources/back.png");


	back_ = std::make_unique<Sprite>();
	back_->Initialize(SpriteCommon::GetInstance(), "resources/back.png");
	back_->SetPosition({ 0.0f, 0.0f });

	ground_ = std::make_unique<Ground>();
	ground_->Initialize();


	//skybox
	skybox_ = std::make_unique<Skybox>();
	skybox_->Initialize(DirectXCommon::GetInstance(), L"resources/rostock_laage_airport_4k.dds");

	


ModelManager::GetInstance()->Initialize(DirectXCommon::GetInstance());
;
ModelManager::GetInstance()->LoadModel("resources/enemy", "enemy.obj");
TextureManager::GetInstance()->LoadTexture("resources/gradationLine.png");
TextureManager::GetInstance()->LoadTexture("resources/circle.png");

//std::unique_ptr<LevelData> levelData = LevelLoader::LoadJsonFile("resources/json", "Untitled3.json");
//
//LevelObjectBuilder::BuildFromJson(
//	levelData.get(),
//	levelObjects_,
//	levelTransforms_,
//	camera_.get()
//);


ground_->SetCamera(camera_.get());

	


Object3dCommon::GetInstance()->SetDefaultCamera(camera_.get());




effectLibrary_ = std::make_unique<ParticleEffectLibrary>();
effectLibrary_->Initialize(DirectXCommon::GetInstance(), SrvManager::GetInstance(), camera_.get());

/*effectLibrary_->SetUsePrimitiveAutoEmit(false);
effectLibrary_->SetUseRingAutoEmit(false);
effectLibrary_->SetUseCylinderAutoEmit(false)*/;


// Register models and transforms

// Load and apply level data




block_ = std::make_unique<Prop>();

ModelManager::GetInstance()->LoadModel("resources/enemy", "enemyTest.obj");
block_->Initialize("enemyTest.obj");
block_->SetCamera(camera_.get());
block_->SetTranslate({ 10.0f, 0.0f, 5.0f });

camera_->SetTranslate({ 0.0f, 5.0f, -14.0f });

PostProcessManager::GetInstance()->ClearEffects();

}

void GameScene::Update() {




#ifdef _DEBUG
	

	ImGui::Begin("Stencil Mask");

	

	ImGui::End();


	ImGui::Begin("Level Objects Rotation");

	for (int i = 0; i < levelTransforms_.size(); ++i) {
		ImGui::Text("Obj %d: pos(%.1f, %.1f, %.1f) scale(%.2f, %.2f, %.2f)",
			i,
			levelTransforms_[i]->translate_.x,
			levelTransforms_[i]->translate_.y,
			levelTransforms_[i]->translate_.z,
			levelTransforms_[i]->scale_.x,
			levelTransforms_[i]->scale_.y,
			levelTransforms_[i]->scale_.z);
	}

	ImGui::End();

	ImGui::Begin("Model Transform");

	Vector3 scale = block_->GetWorldTransform().scale_;
	Vector3 rotate = block_->GetWorldTransform().rotate_;
	Vector3 translate = block_->GetWorldTransform().translate_;

	
	ImGui::DragFloat3("Scale", &scale.x, 0.1f);
	ImGui::DragFloat3("Rotate", &rotate.x, 0.1f);
	ImGui::DragFloat3("Translate", &translate.x, 0.1f);

	block_->SetScale(scale);
	block_->SetRotate(rotate);
	block_->SetTranslate(translate);

	ImGui::End();

#endif
	

	block_->Update();

	back_->Update();

	ground_->Update();

	
	effectLibrary_->Update();
	camera_->Update();
	
	
	for (auto& wt : levelTransforms_) {
		wt->UpdateMatrix();
	}

	for (auto& obj : levelObjects_) {
		obj->Update();
	}

	
	if (input_->TriggerKey(DIK_R)) {
		sceneManager_->ChangeScene("TITLE");

		return;
	}

}

void GameScene::Draw() {
	SpriteCommon::GetInstance()->SetUIPipeline();
	
	//back_->Draw();
	

	//skybox_->Draw(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	Object3dCommon::GetInstance()->CommonDrawSettings();

	SrvManager::GetInstance()->PreDraw();

	ground_->Draw();

	
	for (auto& obj : levelObjects_) {
		obj->Draw();
	}

	//test_plyer->Draw();
	//block_->Draw();

	

	SpriteCommon::GetInstance()->Set3DOverlayPipeline();
	//effectLibrary_->DrawCylinder();
	//player_->HitEffectDraw();
}


void GameScene::Finalize() {
	

	camera_.reset();
}






