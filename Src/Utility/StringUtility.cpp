#include <algorithm>
#include "StringUtility.h"

void StringUtility::DrawStringPlace(std::string _str, int _line, int _posY, int _color, STRING_PLACE _place)
{
	//文字列の長さを取得
	int width = GetDrawStringWidth(_str.c_str(), static_cast<int>(strlen(_str.c_str())));

	//表示するX座標を求める
	int posX = _line;
	switch (_place)
	{
		//左揃えの場合そのまま
	case StringUtility::STRING_PLACE::LEFT:
		break;
		//中央揃えの場合　文字列の長さの半分を引く
	case StringUtility::STRING_PLACE::CENTER:
		posX = _line - width / 2;
		break;
		//右揃えの場合　文字列の長さを引く
	case StringUtility::STRING_PLACE::RIGHT:
		posX = _line - width;
		break;
	default:
		break;
	}
	DrawString(posX, _posY, _str.c_str(), _color);
}

std::string StringUtility::ShowSaveJsonDialog()
{
	// 構造体をゼロ初期化
	OPENFILENAMEW ofn = {};
	wchar_t fileName[MAX_PATH] = L""; // ワイド文字バッファ

	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.lpstrFilter = L"JSONファイル (*.json)\0*.json\0すべてのファイル (*.*)\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"json";

	if (GetSaveFileNameW(&ofn))
	{
		return WideToUtf8(fileName); // UTF-8へ変換
	}

	return ""; // キャンセルされたとき
}

std::string StringUtility::WideToUtf8(const std::wstring& wstr)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);
	result.pop_back(); // null文字を削除
	return result;
}

//std::string StringUtility::GetBtnName(KeyConfig::JOYPAD_BTN btn)
//{
//	std::string name = "";
//	switch (btn)
//	{
//	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT:
//		name = "Bボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN:
//		name = "Aボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_LEFT:
//		name = "Xボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_TOP:
//		name = "Yボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::R_TRIGGER:
//		name = "Rトリガー";
//		break;
//	case KeyConfig::JOYPAD_BTN::L_TRIGGER:
//		name = "Lトリガー";
//		break;
//	case KeyConfig::JOYPAD_BTN::R_BUTTON:
//		name = "Rボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::L_BUTTON:
//		name = "Lボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::START_BUTTON:
//		name = "スタートボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::SELECT_BUTTON:
//		name = "セレクトボタン";
//		break;
//	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_UP:
//		name = "十字キー上";
//		break;
//	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_BOTTOM:
//		name = "十字キー下";
//		break;
//	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_LEFT:
//		name = "十字キー左";
//		break;
//	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_RIGHT:
//		name = "十字キー右";
//		break;
//	case KeyConfig::JOYPAD_BTN::LEFT_STICK:
//		name = "左スティック押し込み";
//		break;
//	case KeyConfig::JOYPAD_BTN::RIGHT_STICK:
//		name = "右スティック押し込み";
//		break;
//	case KeyConfig::JOYPAD_BTN::MAX:
//		name = "割り当てたいボタンを2回押してください";
//		break;
//	}
//	return name;
//}

std::string StringUtility::KeyCodeToStringSmallAlpabet(int keyCode)
{
	std::string ret;
	switch (keyCode)
	{
	case KEY_INPUT_A:
		return "a";
	case KEY_INPUT_B:
		return "b";
	case KEY_INPUT_C:
		return "c";
	case KEY_INPUT_D:
		return "d";
	case KEY_INPUT_E:
		return "e";
	case KEY_INPUT_F:
		return "f";
	case KEY_INPUT_G:
		return "g";
	case KEY_INPUT_H:
		return "h";
	case KEY_INPUT_I:
		return "i";
	case KEY_INPUT_J:
		return "j";
	case KEY_INPUT_K:
		return "k";
	case KEY_INPUT_L:
		return "l";
	case KEY_INPUT_M:
		return "m";
	case KEY_INPUT_N:
		return "n";
	case KEY_INPUT_O:
		return "o";
	case KEY_INPUT_P:
		return "p";
	case KEY_INPUT_Q:
		return "q";
	case KEY_INPUT_R:
		return "r";
	case KEY_INPUT_S:
		return "s";
	case KEY_INPUT_T:
		return "t";
	case KEY_INPUT_U:
		return "u";
	case KEY_INPUT_V:
		return "v";
	case KEY_INPUT_W:
		return "w";
	case KEY_INPUT_X:
		return "x";
	case KEY_INPUT_Y:
		return "y";
	case KEY_INPUT_Z:
		return "z";
	}
	return ret;
}

std::string StringUtility::KeyCodeToStringBigAlpabet(int keyCode)
{
	std::string ret;
	switch (keyCode)
	{
	case KEY_INPUT_A:
		return "A";
	case KEY_INPUT_B:
		return "B";
	case KEY_INPUT_C:
		return "C";
	case KEY_INPUT_D:
		return "D";
	case KEY_INPUT_E:
		return "E";
	case KEY_INPUT_F:
		return "F";
	case KEY_INPUT_G:
		return "G";
	case KEY_INPUT_H:
		return "H";
	case KEY_INPUT_I:
		return "I";
	case KEY_INPUT_J:
		return "J";
	case KEY_INPUT_K:
		return "K";
	case KEY_INPUT_L:
		return "L";
	case KEY_INPUT_M:
		return "M";
	case KEY_INPUT_N:
		return "N";
	case KEY_INPUT_O:
		return "O";
	case KEY_INPUT_P:
		return "P";
	case KEY_INPUT_Q:
		return "Q";
	case KEY_INPUT_R:
		return "R";
	case KEY_INPUT_S:
		return "S";
	case KEY_INPUT_T:
		return "T";
	case KEY_INPUT_U:
		return "U";
	case KEY_INPUT_V:
		return "V";
	case KEY_INPUT_W:
		return "W";
	case KEY_INPUT_X:
		return "X";
	case KEY_INPUT_Y:
		return "Y";
	case KEY_INPUT_Z:
		return "Z";
	}
	return ret;
}

bool StringUtility::Contains(const std::string& text, const std::string& search, bool isComplete)
{
	if(isComplete)
	{
		return text.find(search) != std::string::npos;
	}
	else
	{
		return ToSmallAlpabet(text).find(ToSmallAlpabet(search))!= std::string::npos;
	}
}

std::string StringUtility::ToSmallAlpabet(std::string str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		[](unsigned char c)
		{
			return static_cast<char>(std::tolower(c));
		});

	return str;
}
