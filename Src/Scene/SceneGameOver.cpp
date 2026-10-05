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
#include "SceneGameOver.h"

SceneGameOver::SceneGameOver(void)
{
	auto& resManager = ResourceManager::GetInstance();
	auto menuJson = resManager.GetJsonResource("GameOverJson");
	resManager.GetSound2DResource("GameOverBGM").lock()->Play();
	selectIndex_ = 0;
	maxLayerNum_ = -1;
	json_ = menuJson.lock()->GetData();
	LoadJson();
}

SceneGameOver::~SceneGameOver(void)
{
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.SetIsMousePos(false);
	auto& resManager = ResourceManager::GetInstance();
	resManager.GetSound2DResource("GameOverBGM").lock()->Stop();
}

bool SceneGameOver::Init(void)
{
	return true;
}

void SceneGameOver::Update(void)
{
	if (GetASyncLoadNum() > 0)
	{
		return;
	}
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
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER_MOUSE) && isInBox)
	{
		ColCheckMouse(false);
		UpdateSelectIndex();
		ChangeScene();
		return;
	}
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		ChangeScene();
		return;
	}

}

void SceneGameOver::Draw(void)
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
	}
}

void SceneGameOver::Load(void)
{
	auto& resJson = json_["Resource"];
	for (int i = 0;i < resJson.size();i++)
	{
		ResourceManager::GetInstance().Load(resJson[i]);
	}
}

void SceneGameOver::LoadJson(void)
{
	LoadLogo();
	LoadButton();
}

void SceneGameOver::LoadLogo(void)
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

void SceneGameOver::LoadButton(void)
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

SceneGameOver::DrawStringInfo SceneGameOver::LoadDrawStringInfo(nlohmann::json stringJson)
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

std::vector<SceneGameOver::ButtonInfo> SceneGameOver::GetButtonInfoToLayer(int layerNum)
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

std::vector<SceneGameOver::LogoInfo> SceneGameOver::GetLogoInfoToLayer(int layerNum)
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

void SceneGameOver::ChangeScene(void)
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
	if (next == "Title")
	{
		sceneManager.JumpScene(SceneManager::SCENE_ID::TITLE);
		return;
	}
	else if (next == "Exit")
	{
		Application::GetInstance().ExitGame();
		return;
	}
}

void SceneGameOver::DrawButton(ButtonInfo button)
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

bool SceneGameOver::ColCheckMouse(bool isCheakMove)
{
	KeyConfig& keycon = KeyConfig::GetInstance();
	IntVector2 mouseMove = keycon.GetMouseMove();
	IntVector2 mousePos = keycon.GetMousePos();
	for (auto& button : buttonInfo_)
	{
		Rect rect = button.rect;
		if (CollisionUtility::IsColRect2Point(IntVector2(rect.leftScreen, rect.topScreen), IntVector2(rect.rightScreen, rect.bottomScreen), mousePos))
		{
			if ((mouseMove.x != 0 || mouseMove.y != 0) || !isCheakMove)
			{
				selectIndex_ = button.selectIndex;
			}
			return true;
		}
	}
	return false;
}

void SceneGameOver::UpdateSelectIndex(void)
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
