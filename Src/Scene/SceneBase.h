#pragma once
#include <memory>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Vector.h"
#include "../Utility/Utility.h"
#include "../Object/Button.h"

class SceneBase
{
public:

	SceneBase(void);
	virtual ~SceneBase(void);

	virtual bool Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Load(void);
protected:

	//カメラ
	static constexpr VECTOR CAMERA_POS = { 0.0f,0.0f,-1000.0f };
	static constexpr VECTOR CAMERA_TARGET_POS = { 0.0f,0.0f,0.0f };

	//四角
	struct Rect
	{
		float leftPer;
		float topPer;
		float rightPer;
		float bottomPer;
		int leftScreen;
		int topScreen;
		int rightScreen;
		int bottomScreen;
	};

	struct DrawStringInfo
	{
		std::string fontKey;	//フォントのキー
		int fonthandle;			//フォントのハンドルID
		std::map<std::string, std::string> drawString;	//言語ごとの描画文字
	};

	//ボタンの情報
	struct ButtonInfo
	{
		Button::State state = Button::State::NORMAL;	//状態
		std::string name = "";	//名前
		//std::string imageKey = "";	//画像のキー
		DrawStringInfo drawStrigInfo;	//描画する文字の情報
		std::string nextScene = "";	//次のシーン
		//int imageHandle = -1;	//画像のハンドル
		Rect rect;			//四角の情報
		int selectIndex = -1;	//配列番号
		int layer = -1;
	};

	struct LogoInfo
	{
		std::string name = "";
		std::string imageKey = "";
		int imageHandle = -1;
		FloatVector2 pos;
		int layer = -1;
	};

	struct VirsionInfo
	{
		std::string virsion = "";
		DrawStringInfo drawStrigInfo;	//描画する文字の情報
		VECTOR color = {};
		VECTOR edgeColor = {};
		int layer = -1;
		float sizeMin = 1.0f;
		float sizeMax = 1.0f;
		float sizeChangeRate = 1.0f;
		float rotation = 0.0f;
		FloatVector2 centerPos = {};
		IntVector2 centerPosScreen = {};
	};

	void UpdateIsPose(void);
private:

	
};