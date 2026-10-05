#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Utility/JsonUtility.h"
#include "../Utility/ColorUtility.h"
#include "../Utility/CollisionUtility.h"
#include "../Manager/Scenemanager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource/JsonResource.h"
#include "../Manager/Resource/FontResource.h"
#include "../Manager/Resource/ImageResource.h"
#include "../Manager/Resource/Sound/Sound2DResource.h"
#include "SceneTitle.h"


SceneTitle::SceneTitle(void)
{
	auto& resManager = ResourceManager::GetInstance();
	auto titleJson = resManager.GetJsonResource("TitleJson");
	resManager.GetSound2DResource("TitleBGM").lock()->Play();
	time_ = 0.0f;
	selectIndex_ = 0;
	maxLayerNum_ = -1;
	json_ = titleJson.lock()->GetData();
	LoadJson();
}

SceneTitle::~SceneTitle(void)
{
	auto& resManager = ResourceManager::GetInstance();
	resManager.GetSound2DResource("TitleBGM").lock()->Stop();
}

//初期化処理(初回の1度のみ実行される)
bool SceneTitle::Init(void)
{
	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	auto& resManager = ResourceManager::GetInstance();
	if (GetASyncLoadNum() > 0)
	{
		return;
	}
	int prevSelectIndex = selectIndex_;
	time_ += SceneManager::GetInstance().GetDeltaTime();
	auto& keyConfig = KeyConfig::GetInstance();
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		selectIndex_--;
	}
	else if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		selectIndex_++;
	}
	bool isInBox = ColCheckMouse();
	UpdateSelectIndex();
	if (selectIndex_ != prevSelectIndex)
	{
		resManager.GetSound2DResource("MoveSelectSE").lock()->Play();
	}
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER_MOUSE) && isInBox)
	{
		resManager.GetSound2DResource("EnterSE").lock()->Play();
		ColCheckMouse(false);
		UpdateSelectIndex();
		ChangeScene();
		return;
	}
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		resManager.GetSound2DResource("EnterSE").lock()->Play();
		ChangeScene();
		return;
	}
}
//描画処理
void SceneTitle::Draw(void)
{
	for (int i = 0; i <= maxLayerNum_; i++)
	{
		auto logos = GetLogoInfoToLayer(i);
		for (auto& logo : logos)
		{
			int x = static_cast<int>(logo.pos.x * Application::SCREEN_SIZE_X);
			int y = static_cast<int>(logo.pos.y * Application::SCREEN_SIZE_Y);
			//ロゴの描画
			DrawRotaGraph(x, y, 1.0f, 0.0f, logo.imageHandle, true);
		}
		auto buttons = GetButtonInfoToLayer(i);
		for (auto& button : buttons)
		{
			DrawButton(button);
		}
		auto virsions = GetVirsionInfoToLayer(i);
		for (auto& virsion : virsions)
		{
			DrawVirsion(virsion);
		}
	}
}

void SceneTitle::Load(void)
{
	auto& resJson = json_["Resource"];
	for (int i = 0;i < resJson.size();i++)
	{
		ResourceManager::GetInstance().Load(resJson[i]);
	}
}

void SceneTitle::LoadJson(void)
{
	LoadLogo();
	LoadVirsion();
	LoadButton();
}

void SceneTitle::LoadLogo(void)
{
	auto& logosJson = json_["Logo"];
	//ロゴの情報を取得
	for (int i = 0; i < logosJson["List"].size();i++)
	{
		LogoInfo info;
		auto& logoJson = logosJson["List"][i];
		info.name = logoJson["Name"];
		info.imageKey = logoJson["Image"];
		info.imageHandle = ResourceManager::GetInstance().GetImageResource(info.imageKey).lock()->GetHandleId();
		info.pos = JsonUtility::GetPosToFloat2D(logoJson);
		info.layer = logoJson["Layer"];
		maxLayerNum_ = maxLayerNum_ < info.layer ? info.layer : maxLayerNum_;
		logoInfo_.push_back(info);
	}
}

void SceneTitle::LoadButton(void)
{
	auto& buttonsJson = json_["Buttons"];
	//ボタンの情報
	for (int i = 0; i < buttonsJson["List"].size();i++)
	{
		ButtonInfo info;
		auto& buttonJson = buttonsJson["List"][i];
		info.state = Button::State::NORMAL;
		info.name = buttonJson["Name"];
		//info.imageKey = buttonJson["Image"];
		info.drawStrigInfo = LoadDrawStringInfo(buttonJson["String"]);
		info.nextScene = buttonJson["NextScene"];
		info.selectIndex = buttonJson["SelectIndex"];
		info.layer = buttonJson["Layer"];
		maxLayerNum_ = maxLayerNum_ < info.layer ? info.layer : maxLayerNum_;
		//info.imageHandle = ResourceManager::GetInstance().GetImageResource(info.imageKey).lock()->GetHandleId();
		//四角の情報
		auto& rectJson = buttonJson["Rect"];
		info.rect.leftPer = rectJson["Left"];
		info.rect.topPer = rectJson["Top"];
		info.rect.rightPer = rectJson["Right"];
		info.rect.bottomPer = rectJson["Bottom"];
		info.rect.leftScreen = Application::SCREEN_SIZE_X * info.rect.leftPer;
		info.rect.rightScreen = Application::SCREEN_SIZE_X * info.rect.rightPer;
		info.rect.topScreen = Application::SCREEN_SIZE_Y * info.rect.topPer;
		info.rect.bottomScreen = Application::SCREEN_SIZE_Y * info.rect.bottomPer;
		buttonInfo_.push_back(info);
	}
}

void SceneTitle::LoadVirsion(void)
{
	auto& virsionsJson = json_["Virsion"];
	//ボタンの情報
	for (int i = 0; i < virsionsJson["List"].size();i++)
	{
		VirsionInfo info;
		auto& virsionJson = virsionsJson["List"][i];
		info.virsion = virsionJson["Virsion"];
		info.drawStrigInfo = LoadDrawStringInfo(virsionJson["String"]);
		info.color = JsonUtility::GetColorFloat(virsionJson["Color"]);
		info.edgeColor = JsonUtility::GetColorFloat(virsionJson["EdgeColor"]);
		info.layer = virsionJson["Layer"];
		maxLayerNum_ = maxLayerNum_ < info.layer ? info.layer : maxLayerNum_;
		info.sizeMin = virsionJson["Size"]["Min"];
		info.sizeMax = virsionJson["Size"]["Max"];
		info.sizeChangeRate = virsionJson["Size"]["ChangeRate"];
		info.rotation = Utility::Deg2RadF(virsionJson["Rotation"]);

		info.centerPos = JsonUtility::GetPosToFloat2D(virsionJson["CenterPos"]);
		info.centerPosScreen.x = info.centerPos.x * Application::SCREEN_SIZE_X;
		info.centerPosScreen.y = info.centerPos.y * Application::SCREEN_SIZE_Y;
		virsionInfo_.push_back(info);
	}
}

SceneTitle::DrawStringInfo SceneTitle::LoadDrawStringInfo(nlohmann::json stringJson)
{
	DrawStringInfo ret;
	ret.fontKey = stringJson["Font"];
	ret.fonthandle = ResourceManager::GetInstance().GetFontResource(ret.fontKey).lock()->GetHandleId();
	for (auto& str : stringJson["DrawString"].items())
	{
		ret.drawString.emplace(str.key(), str.value());
	}
	return ret;
}

std::vector<SceneTitle::ButtonInfo> SceneTitle::GetButtonInfoToLayer(int layerNum)
{
	std::vector<ButtonInfo>ret;
	for (auto& button : buttonInfo_)
	{
		if (button.layer = layerNum)
		{
			ret.push_back(button);
		}
	}
	return ret;
}

std::vector<SceneTitle::LogoInfo> SceneTitle::GetLogoInfoToLayer(int layerNum)
{
	std::vector<LogoInfo>ret;
	for (auto& logo : logoInfo_)
	{
		if (logo.layer = layerNum)
		{
			ret.push_back(logo);
		}
	}
	return ret;
}

std::vector<SceneTitle::VirsionInfo> SceneTitle::GetVirsionInfoToLayer(int layerNum)
{
	std::vector<VirsionInfo>ret;
	for (auto& virsion : virsionInfo_)
	{
		if (virsion.layer = layerNum)
		{
			ret.push_back(virsion);
		}
	}
	return ret;
}


void SceneTitle::ChangeScene(void)
{
	std::string next = "";
	for (auto& button : buttonInfo_)
	{
		if (button.selectIndex == selectIndex_)
		{
			next = button.nextScene;
		}
	}
	auto& sceneManager = SceneManager::GetInstance();
	if (next == "Game")
	{
		sceneManager.ChangeScene(SceneManager::SCENE_ID::GAME,true);
		return;
	}
	else if (next == "SkinCreate")
	{
		sceneManager.ChangeScene(SceneManager::SCENE_ID::MAKE_SKIN, true);
		return;
	}
	else if (next == "SkinSelect")
	{
		sceneManager.ChangeScene(SceneManager::SCENE_ID::SELECT_SKIN, true);
		return;
	}
	else if (next == "Setting")
	{
		//現在実装なし
		return;
		sceneManager.ChangeScene(SceneManager::SCENE_ID::SETTING, true);
		return;
	}
	else if (next == "Exit")
	{
		Application::GetInstance().ExitGame();
		return;
	}

}

void SceneTitle::DrawButton(ButtonInfo button)
{
	//ボタンのテキスト描画
	auto& resourceManager = ResourceManager::GetInstance();
	const auto& resource = resourceManager.GetFontResource(button.drawStrigInfo.fontKey).lock();
	//フォントのハンドルを取得
	int fontHandle = resource->GetHandleId();

	Button::GetInstance().Draw(IntVector2(button.rect.leftScreen, button.rect.topScreen),
		IntVector2(button.rect.rightScreen, button.rect.bottomScreen),
		button.state, fontHandle, button.drawStrigInfo.drawString["Japanese"]);
}

void SceneTitle::DrawVirsion(VirsionInfo virsion)
{
	auto& resourceManager = ResourceManager::GetInstance();
	const auto& resource = resourceManager.GetFontResource(virsion.drawStrigInfo.fontKey).lock();
	//フォントのサイズを取得
	//フォントのハンドルを取得
	int fontHandle = resource->GetHandleId();
	//色を取得
	int col = ColorUtility::ColorChange(virsion.color);
	int edgeCol = ColorUtility::ColorChange(virsion.edgeColor);
	std::string str = virsion.drawStrigInfo.drawString["Japanese"] + virsion.virsion;
	//float sizeRate = sin(time_) * ((virsion.sizeMax - virsion.sizeMin) / (1.0f - -1.0f)) + (virsion.sizeMax - virsion.sizeMin) / 2;
	float sizeRate = virsion.sizeMin + (virsion.sizeMax - virsion.sizeMin) * ((sinf(time_* virsion.sizeChangeRate) + 1.0f) * 0.5f);
	float width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(str.size()), fontHandle);
	float size = resource->GetSize();
	//DrawRotaStringToHandle(center.x - width / 2, center.y - size / 2, sizeRate,sizeRate,center.x,center.y,virsion.rotation,col,fontHandle,col,true, str.c_str());
	DrawRotaStringToHandle(virsion.centerPosScreen.x, virsion.centerPosScreen.y, sizeRate, sizeRate,width * 0.5,size * 0.5, virsion.rotation, col, fontHandle, edgeCol, false, str.c_str());
}

bool SceneTitle::ColCheckMouse(bool isCheakMove)
{
	KeyConfig& keycon = KeyConfig::GetInstance();
	IntVector2 mouseMove = keycon.GetMouseMove();
	IntVector2 mousePos = keycon.GetMousePos();
	for (auto& button : buttonInfo_)
	{
		Rect rect = button.rect;
		if (CollisionUtility::IsColRect2Point(IntVector2(rect.leftScreen, rect.topScreen), IntVector2(rect.rightScreen, rect.bottomScreen), mousePos))
		{
			if ((mouseMove.x != 0 || mouseMove.y != 0)|| !isCheakMove)
			{
				selectIndex_ = button.selectIndex;
			}
			return true;
		}
	}
	return false;
}

void SceneTitle::UpdateSelectIndex(void)
{
	selectIndex_ = (selectIndex_ + static_cast<int>(buttonInfo_.size())) % static_cast<int>(buttonInfo_.size());
	for (auto& button : buttonInfo_)
	{
		if (button.selectIndex == selectIndex_)
		{
			button.state = Button::State::SELECT;
		}
		else
		{
			button.state = Button::State::NORMAL;
		}
	}
}
