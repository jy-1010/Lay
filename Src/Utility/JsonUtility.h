#pragma once
#include <string>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Vector.h"

class JsonUtility
{
public:

	/// <summary>
	/// そのキーが含まれているか
	/// </summary>
	/// <param name="json">json情報</param>
	/// <param name="key">キー</param>
	/// <returns></returns>
	static bool IsContain(const nlohmann::json& json,const std::string& key);

	/// <summary>
	/// 2D座標情報を取得する
	/// </summary>
	/// <param name="json">json情報</param>
	/// <returns>座標</returns>
	static FloatVector2 GetPosToFloat2D(const nlohmann::json& json);

	/// <summary>
	/// 2D座標情報を取得する
	/// </summary>
	/// <param name="json">json情報</param>
	/// <returns>座標</returns>
	static FloatVector2 GetPosToInt2D(const nlohmann::json& json);

	/// <summary>
	/// 3D座標情報を取得
	/// </summary>
	/// <param name="json">json情報</param>
	/// <returns>座標</returns>
	static VECTOR GetPosToFloat3D(const nlohmann::json& json);

	/// <summary>
	/// 3D座標情報を取得
	/// </summary>
	/// <param name="json">json情報</param>
	/// <returns>座標</returns>
	static IntVector3 GetPosToInt3D(const nlohmann::json& json);

	/// <summary>
	/// 色情報をフロートで取得
	/// </summary>
	/// <param name="json">json情報</param>
	/// <returns>色情報</returns>
	static VECTOR GetColorFloat(const nlohmann::json& json);
};

