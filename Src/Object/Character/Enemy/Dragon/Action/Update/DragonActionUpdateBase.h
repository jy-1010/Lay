#pragma once
#include <memory>
#include <string>
#include "../../../../../../Lib/nlohmann/json.hpp"
#include "../../../../../Common/Transform.h"

class DragonAction;

class DragonActionUpdateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dragonTrans">ドラゴンのトランスフォーム</param>
	DragonActionUpdateBase(std::weak_ptr<Transform>dragonTrans,nlohmann::json json,DragonAction& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~DragonActionUpdateBase(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void) = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void) = 0;

protected:

	//親クラス
	DragonAction& parent_;

	//ドラゴンのトランスフォーム
	std::weak_ptr<Transform>dragonTrans_;

	// アクション別のJSON
	nlohmann::json json_;

	// jsonのデータをロードする
	virtual void Load(void) = 0;
};