#include "Button.h"
#include "../Utility/ColorUtility.h"
#include "../Utility/JsonUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource/JsonResource.h"

Button* Button::instance_ = nullptr;

void Button::CreateInstance(void)
{	
	//インスタンスの初期生成
	if (instance_ == nullptr)
	{
		instance_ = new Button();
	}
	instance_->Init();
}

Button& Button::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		Button::CreateInstance();
	}
	return *instance_;
}

void Button::Init(void)
{
	Load();
}

void Button::Draw(IntVector2 leftUp, IntVector2 rightDown, State state, int fontHandle, std::string drawString)
{
	IntVector2 center = {
		static_cast<int>((leftUp.x + rightDown.x) / 2),
		static_cast<int>((leftUp.y + rightDown.y) / 2)
	};
	DrawOutLine(leftUp, rightDown, state);

	//フォントのサイズを取得
	int size = GetFontSizeToHandle(fontHandle);
	int col = ColorUtility::ColorChange(textColor_[state]);
	//文字列の長さを取得する
	int width = GetDrawStringWidthToHandle(drawString.c_str(), static_cast<int>(drawString.size()), fontHandle);
	DrawStringToHandle(center.x - width / 2, center.y - size / 2, drawString.c_str(), col, fontHandle);
}

void Button::Destroy(void)
{
}

void Button::Load(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto& json = resourceManager.GetJsonResource("ButtonJson").lock()->GetData();
	auto& buttonColor = json["Color"];
	//色情報の取得
	backColor_[State::NORMAL] = JsonUtility::GetColorFloat(buttonColor["Normal"]["Back"]);
	backColor_[State::SELECT] = JsonUtility::GetColorFloat(buttonColor["Selected"]["Back"]);
	backColor_[State::PRESS] = JsonUtility::GetColorFloat(buttonColor["Pressed"]["Back"]);
	textColor_[State::NORMAL] = JsonUtility::GetColorFloat(buttonColor["Normal"]["Text"]);
	textColor_[State::SELECT] = JsonUtility::GetColorFloat(buttonColor["Selected"]["Text"]);
	textColor_[State::PRESS] = JsonUtility::GetColorFloat(buttonColor["Pressed"]["Text"]);

	//アウトラインの色を取得
	auto& outlineColor = buttonColor["OutLine"];
	blackInfo_.color = JsonUtility::GetColorFloat(outlineColor["Black"]);
	blackInfo_.thickness = outlineColor["Black"]["Thickness"];
	lightInfo_.color = JsonUtility::GetColorFloat(outlineColor["Light"]);
	lightInfo_.thickness = outlineColor["Light"]["Thickness"];
	darkInfo_.color = JsonUtility::GetColorFloat(outlineColor["Dark"]);
	darkInfo_.thickness = outlineColor["Dark"]["Thickness"];
}

void Button::DrawOutLine(IntVector2 leftUp, IntVector2 rightDown, State state)
{
	//色を変換する
	int black = ColorUtility::ColorChange(blackInfo_.color);
	int light = ColorUtility::ColorChange(lightInfo_.color);
	int dark = ColorUtility::ColorChange(darkInfo_.color);
	int back = ColorUtility::ColorChange(backColor_[state]);

	//座標返還

	//大外の黒を描画
	DrawBoxAA(leftUp.x, leftUp.y, rightDown.x, rightDown.y, black, false, blackInfo_.thickness);
	//うちの明るい部分を描画
	DrawLine(leftUp.x + 1, leftUp.y + 1, rightDown.x - 1, leftUp.y + 1, light, lightInfo_.thickness);
	DrawLine(leftUp.x + 1, leftUp.y + 1, leftUp.x + 1, rightDown.y - 1, light, lightInfo_.thickness);
	//うちの暗い部分を描画
	DrawLine(leftUp.x + 1, rightDown.y - 1, rightDown.x - 1, rightDown.y - 1, dark, darkInfo_.thickness);
	DrawLine(rightDown.x - 1, leftUp.y + 1, rightDown.x - 1, rightDown.y - 1, dark, darkInfo_.thickness);
	//背景色の描画
	DrawBox(leftUp.x + 2, leftUp.y + 2, rightDown.x - 2, rightDown.y - 2, back, true);
}
