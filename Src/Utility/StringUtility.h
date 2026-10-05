#pragma once
#include <string>
#include "../Manager/KeyConfig.h"

class StringUtility
{
public:

	enum class STRING_PLACE	//文字列を揃える位置
	{
		LEFT,		//左揃え
		CENTER,		//中央揃え
		RIGHT,		//右揃え
	};

	/// <summary>
	/// 文字列を指定位置に揃えて描画
	/// </summary>
	/// <param name="_str">描画する文字列</param>
	/// <param name="_line">揃えるX座標　左揃え：左の座標　中央揃え：中央の座標　右揃え：右の座標</param>
	/// <param name="_posY">描画するY座標</param>
	/// <param name="_color">描画する色コード</param>
	/// <param name="_place">揃える位置</param>
	static void DrawStringPlace(std::string _str, int _line, int _posY, int _color, STRING_PLACE _place);

	/// <summary>
	/// JSON保存ダイアログを表示し、パスをUTF-8文字列で返す
	/// </summary>
	/// <returns>文字列を返す</returns>
	static std::string ShowSaveJsonDialog();

	/// <summary>
	/// ワイド文字列をUTF-8に変換する
	/// </summary>
	static std::string WideToUtf8(const std::wstring& wstr);

	/// <summary>
	/// ボタンの名前の文字列を取得
	/// </summary>
	/// <param name="btn">ボタンの種類</param>
	/// <returns>ボタンの文字列</returns>
	//static std::string GetBtnName(KeyConfig::JOYPAD_BTN btn);

	/// <summary>
	/// キーコードを小文字英語んstringに変換
	/// </summary>
	/// <param name="keycode">キーコード</param>
	/// <returns></returns>
	static std::string KeyCodeToStringSmallAlpabet(int keyCode);

	/// <summary>
	/// キーコードを大文字英語stringに変換
	/// </summary>
	/// <param name="keycode">キーコード</param>
	/// <returns></returns>
	static std::string KeyCodeToStringBigAlpabet(int keyCode);

	/// <summary>
	/// 文字列の中に指定の文字列が入っているか(アルファベット限定)
	/// </summary>
	/// <param name="text">検索される全体の文字列</param>
	/// <param name="search">検索する文字列</param>
	/// <param name="isComplete">大文字小文字を判定するか(true:する)</param>
	/// <returns>true:入っている</returns>
	static bool Contains(const std::string& text, const std::string& search, bool isComplete = true);

	/// <summary>
	/// 文字列を全て小文字にする
	/// </summary>
	/// <param name="str">文字列</param>
	/// <returns>小文字の文字列</returns>
	static std::string ToSmallAlpabet(std::string str);
};

