#include "../../Application.h"
#include "../../Utility/ColorUtility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource/FontResource.h"
#include "../../Manager/Resource/Sound/Sound2DResource.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/Camera.h"
#include "../../Object/Character/Player/Skin/SkinManager.h"
#include "../../Object/Character/Player/Player.h"
#include "SceneSelectSkin.h"

SceneSelectSkin::SceneSelectSkin(void)
{
	auto& resManager = ResourceManager::GetInstance();
	resManager.GetSound2DResource("SkinBGM").lock()->Play();
	selectSkinNum_ = 1;
	screenSize_ = { Application::SCREEN_SIZE_X / SCREEN_DRAW_NUM, Application::SCREEN_SIZE_Y };
	auto& skinManager = SkinManager::GetInstance();
	std::vector<std::string> keys = skinManager.GetSkinNameAll();
	for (auto& key : keys)
	{
		AddScreenInfo(key);
	}
	previewPlayer_ = std::make_shared<Player>("");
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW_ROTATION);
	camera.SetFollow(previewPlayer_->GetTransform().lock());
}

SceneSelectSkin::~SceneSelectSkin(void)
{
	for (auto& screen : skinPreviewScreens_)
	{
		DeleteGraph(screen.screenHandle);
	}
	auto& resManager = ResourceManager::GetInstance();
	resManager.GetSound2DResource("SkinBGM").lock()->Stop();
}

bool SceneSelectSkin::Init(void)
{
	return true;
}

void SceneSelectSkin::Update(void)
{
	auto& resManager = ResourceManager::GetInstance();
	int prevSkinNum = selectSkinNum_;
	previewPlayer_->Update();
	auto& keyConfig = KeyConfig::GetInstance();
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		selectSkinNum_--;
	}
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		selectSkinNum_++;
	}
	selectSkinNum_ = (selectSkinNum_ + static_cast<int>(skinPreviewScreens_.size())) % static_cast<int>(skinPreviewScreens_.size());
	if (selectSkinNum_ != prevSkinNum)
	{
		resManager.GetSound2DResource("MoveSelectSE").lock()->Play();
	}
	for (auto& info : skinPreviewScreens_)
	{
		info.isDraw = IsDrawScren(info.skinNum);
		info.pos = GetScreenPos(info.skinNum);
	}
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		resManager.GetSound2DResource("EnterSE").lock()->Play();
		SkinManager::GetInstance().SetSelectedSkinName(GetSelectSkinName());
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME,true);
	}
	UpdateIsPose();
}

void SceneSelectSkin::Draw(void)
{
	int font = ResourceManager::GetInstance().GetFontResource("DefaultFont").lock()->GetHandleId();
	for (auto& screen : skinPreviewScreens_)
	{
		if (!screen.isDraw)
		{
			//描画しない場合次に移る
			continue;
		}
		//描画する場合
		//それぞれのスクリーンに描画
		SetDrawScreen(screen.screenHandle);
		ClearDrawScreen();
		//カメラの設定
		SceneManager::GetInstance().GetCamera().CameraSetting();
		//プレイヤーの描画
		previewPlayer_->SetSkinHandle(screen.skinHandle);
		previewPlayer_->Draw();
		if (selectSkinNum_ == screen.skinNum)
		{
			//今選択中のもの
			DrawBoxAA(0, 0, screenSize_.x, screenSize_.y, ColorUtility::YELLOW, false, SELECT_EDGE_SIZE);
		}
		//int width =GetDrawStringWidth(screen.skinName.c_str(), GetStringLength(screen.skinName.c_str()));
		std::string str = screen.skinName;

		int width =GetDrawStringWidthToHandle(screen.skinName.c_str(), GetStringLength(screen.skinName.c_str()),font);
		DrawStringToHandle(screenSize_.x / 2 - width / 2, 30, screen.skinName.c_str(), 0xffffff,font);

		//メインスクリーンに描画
		SetDrawScreen(SceneManager::GetInstance().GetMainScreen());
		DrawGraph(screen.pos.x, screen.pos.y, screen.screenHandle, true);
	}
}

void SceneSelectSkin::Load(void)
{
}

void SceneSelectSkin::AddScreenInfo(std::string skinName)
{
	auto& skinManager = SkinManager::GetInstance();
	ScreenInfo info;
	info.screenHandle = MakeScreen(screenSize_.x,screenSize_.y,true);
	info.skinHandle = skinManager.GetHandleId(skinName);
	info.skinName = skinName;
	info.skinNum = static_cast<int>(skinPreviewScreens_.size());
	//info.isDraw = IsDrawScren(info.skinNum);
	skinPreviewScreens_.push_back(info);
}

bool SceneSelectSkin::IsDrawScren(int skinNum)
{
	int skinMaxNum = static_cast<int>(skinPreviewScreens_.size());
	int min = selectSkinNum_ - static_cast<int>(SCREEN_DRAW_NUM / 2);
	min = (min + skinMaxNum) % skinMaxNum;
	int max = selectSkinNum_ + static_cast<int>(SCREEN_DRAW_NUM / 2);
	max = max % skinMaxNum;
	if (min < max)
	{
		if (min <= skinNum && skinNum <= max)
		{
			return true;
		}
	}
	else
	{
		if (min >= skinNum || skinNum >= max)
		{
			return true;
		}
	}
	return false;
}

IntVector2 SceneSelectSkin::GetScreenPos(int skinNum)
{
	IntVector2 pos = { 0,0 };
	if (!IsDrawScren(skinNum))
	{
		//描画しないならリターンする
		return pos;
	}
	int skinMaxNum = static_cast<int>(skinPreviewScreens_.size());
	int min = selectSkinNum_ - static_cast<int>(SCREEN_DRAW_NUM / 2);
	min = (min + skinMaxNum) % skinMaxNum;
	int max = selectSkinNum_ + static_cast<int>(SCREEN_DRAW_NUM / 2);
	max = max % skinMaxNum;
	if (min > max)
	{
		//minかmaxどちらかが基準値を超えた場合
		max += skinMaxNum;
		if (min > skinNum)
		{
			skinNum += skinMaxNum;
		}
	}
	//左から何個目かを取得する
	int sub = skinNum - min;
	pos.x = sub * screenSize_.x;
	return pos;
}

std::string SceneSelectSkin::GetSelectSkinName(void)
{
	for (auto& screen : skinPreviewScreens_)
	{
		if (screen.skinNum == selectSkinNum_)
		{
			return screen.skinName;
		}
	}
	return "";
}
