#include<DxLib.h>
#include"../Application.h"
#include "../Utility/Utility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/CollisionManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/Resource/ImageResource.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
#include"../Manager/Camera.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Enemy/Dragon/Dragon.h"
#include "../Object/Character/Player/Skin/SkinManager.h"
#include "../Object/Item/Block/BlockInfo.h"
#include"SceneGame.h"

SceneGame::SceneGame(void)
{
	stage_ = std::make_unique<Stage>();
	player_ = std::make_unique<Player>(SkinManager::GetInstance().GetSelectedSkinName(), stage_->GetBlockSize());
	player_->SetPos(stage_->GetPlayerSpawnPos());
	dragon_ = std::make_unique<Dragon>(*player_,stage_->GetBlockSize());
	dragon_->SetPos(stage_->GetEnemySpawnPos());
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);
	camera.SetFollow(player_->GetHeadTransform().lock());
	camera.SetIsMousePos(true);
}

SceneGame::~SceneGame(void)
{
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.SetIsMousePos(false);
}


//初期化処理(初回の1度のみ実行される)
bool SceneGame::Init(void)
{
	SceneBase::Init();
	
	return true;
}

//更新処理
void SceneGame::Update(void)
{
	if (stage_->IsLoading())
	{
		//ロード中は更新しない
		return;
	}
	auto& camera = SceneManager::GetInstance().GetCamera();
	if (!camera.GetIsSetMousePos())
	{
		camera.SetIsMousePos(true);
	}
	KeyConfig& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CHANGE_CAMERA_MODE))
	{
		auto cameraMode = camera.GetMode();
		if (cameraMode == Camera::MODE::FOLLOW)
		{
			camera.ChangeMode(Camera::MODE::FPS);
		}
		else if (cameraMode == Camera::MODE::FPS)
		{
			camera.ChangeMode(Camera::MODE::FOLLOW);
		}
	}
	player_->Update();
	stage_->Update();
	dragon_->Update();
	player_->ApplyVertex();
	UpdateIsPose();
	CollisionManager::GetInstance().Update();
	if (dragon_->IsDead())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR, true);
		return;
	}
	if (player_->IsDead())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER, true);
		return;
	}
}

//描画処理
void SceneGame::Draw(void)
{
	if (stage_->IsLoading())
	{
		//ロード中の描画処理
		return;
	}
	player_->Draw();
	dragon_->Draw();
	stage_->Draw();

	player_->UIDraw();
	dragon_->UIDraw();
	stage_->UIDraw();
	if (SceneManager::GetInstance().GetCamera().GetMode() == Camera::MODE::FPS)
	{
		DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 1.0f, 0.0f, ResourceManager::GetInstance().GetImageResource("CrossHairImage").lock()->GetHandleId(), true);
	}
}

void SceneGame::DebugDraw(void)
{
}

float SceneGame::GetBlockSize(void)
{
	return stage_->GetBlockSize();
}

IntVector3 SceneGame::WorldPosToMapPos(const VECTOR& worldPos) const
{
	return stage_->WorldPosToMapPos(worldPos);
}

IntVector3 SceneGame::MapPosToChunkPos(const IntVector3& mapPos) const
{
	return stage_->MapPosToChunkPos(mapPos);
}

IntVector3 SceneGame::WorldPosToChunkPos(const VECTOR& worldPos) const
{
	return stage_->WorldPosToChunkPos(worldPos);
}

VECTOR SceneGame::MapPosToWorldPos(const IntVector3& mapPos) const
{
	return stage_->MapPosToWorldPos(mapPos);
}

IntVector3 SceneGame::ChunkPosToMapPos(const IntVector3& chunkPos) const
{
	return stage_->ChunkPosToMapPos(chunkPos);
}

VECTOR SceneGame::ChunkPosToWorldPos(const IntVector3& chunkPos) const
{
	return stage_->ChunkPosToWorldPos(chunkPos);
}


