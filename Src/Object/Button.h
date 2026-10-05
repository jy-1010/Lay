#pragma once
#include <string>
#include <map>
#include "../Common/Vector.h"

class Button 
{
public:

	//ボタンの状態
	enum class State
	{
		NORMAL,	//通常
		SELECT,	//選択中
		PRESS,	//決定
	};

	/// <summary>
	/// インスタンスを生成する
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static Button& GetInstance(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// ボタンの描画
	/// </summary>
	/// <param name="leftUp">左上座標</param>
	/// <param name="rightDown">右下座標</param>
	/// <param name="state">ボタンの状態</param>
	/// <param name="fontHandle">フォントのハンドル</param>
	/// <param name="drawString">ボタンの中に描画する文字</param>
	void Draw(IntVector2 leftUp, IntVector2 rightDown, State state, int fontHandle = -1,std::string drawString = "");

	/// <summary>
	/// インスタンスの破棄
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);
private:

	struct OutLineInfo
	{
		VECTOR color = {};	//色
		float thickness = -1.0f;	//太さ
	};

	static Button* instance_;

	//ボタン背景色
	std::map<State, VECTOR> backColor_;
	//ボタンテキスト色
	std::map<State, VECTOR> textColor_;

	//アウトラインの色
	OutLineInfo blackInfo_;
	OutLineInfo lightInfo_;
	OutLineInfo darkInfo_;

	//ロード
	void Load(void);

	//アウトラインの描画
	void DrawOutLine(IntVector2 leftUp, IntVector2 rightDown, State state);

	Button(void) = default;
	~Button(void) = default;

};

