#include<DxLib.h>
#include"../Application.h"
#include "../Utility/Utility.h"
#include "../Common/SingletonRegistry.h"
#include"../Manager/SceneManager.h"
#include"../Manager/CollisionManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/Resource/ImageResource.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
#include"../Manager/Camera.h"
#include"../Object/Player/PlayerManager.h"
#include"SceneGame.h"

SceneGame::SceneGame(void)
{
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_DIAGONAL);
	//camera.SetIsMousePos(true);
}

SceneGame::~SceneGame(void)
{
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.SetIsMousePos(false);

	//ゲーム終了時に破棄するシングルトンを削除
	SingletonRegistry::GetInstance().Delete(SingletonRegistry::DESTROY_TIMING::GAME_END);
}


//初期化処理(初回の1度のみ実行される)
bool SceneGame::Init(void)
{
	SceneBase::Init();

	//プレイヤーの生成
	PlayerManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::GAME_END);
	PlayerManager::GetInstance().Init();
	
	return true;
}

//更新処理
void SceneGame::Update(void)
{
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
	UpdateIsPose();
	CollisionManager::GetInstance().Update();
	PlayerManager::GetInstance().Update();
}

//描画処理
void SceneGame::Draw(void)
{
	if (SceneManager::GetInstance().GetCamera().GetMode() == Camera::MODE::FPS)
	{
		DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 1.0f, 0.0f, ResourceManager::GetInstance().GetImageResource("CrossHairImage").lock()->GetHandleId(), true);
	}

	PlayerManager::GetInstance().Draw();
}

void SceneGame::DebugDraw(void)
{
}