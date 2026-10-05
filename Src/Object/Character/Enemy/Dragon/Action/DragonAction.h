#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../../../../../Lib/nlohmann/json.hpp"
#include "../../../../Common/Transform.h"

class DragonActionUpdateBase;
class DragonActionManager;

class DragonAction
{
public:

	enum class Action
	{
		NONE = 0,
		FLY_MOVE = 1,	// 飛行移動
		FLY_BREATH_ATTACK = 2,	// 飛行ブレス攻撃
		LANDING = 3,	// 着地
		LAND_ATTACK = 4,	// 地上攻撃
		TAKE_FLIGHT = 5,	// 飛行開始
		FLY_PLAYER_ATTACK = 6,	// プレイヤーへの飛行攻撃
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	DragonAction(nlohmann::json jsonData,std::shared_ptr<Transform> transform,float blockSize,const DragonActionManager& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonAction(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);
	
	/// <summary>
	/// UI描画処理
	/// </summary>
	/// <param name=""></param>
	void UIDraw(void);

	/// <summary>
	/// IDを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetId(void) const { return id_; }

	/// <summary>
	/// 次のアクションのIDを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetNextActionId(void) const { return nextActionId_; }

	/// <summary>
	/// アクションが終了したか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsEnd(void) const { return duration_ < time_; }

	/// <summary>
	/// 名前を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::string GetName(void) { return name_; }

	/// <summary>
	/// ブロックの大きさを取得
	/// </summary>
	const float GetBlockSize(void) const { return blockSize_; }

	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const VECTOR GetPlayerPos(void)const;

private:

	struct NextActionInfo
	{
		int id;	//次のアクションのID
		int weight;	//次のアクションの重み(優先度)
		Action nextAction;	//次のアクションの
	};

	const DragonActionManager& parent_;

	// ドラゴンのトランスフォーム
	std::weak_ptr<Transform> transform_;

	std::unique_ptr< DragonActionUpdateBase>update_;

	// アクションの情報json
	nlohmann::json jsonData_;

	//ブロックの大きさ
	float blockSize_;

	// アクションのID
	int id_;

	// アクション
	Action action_;

	// アクションの名前
	std::string name_;

	// アクションの持続時間
	float duration_;

	// アクションの経過時間
	float time_;

	// 次のアクションのリスト
	std::vector<NextActionInfo> nextActions_;

	// 次のアクションの重みの合計
	int sumWeight_;	

	//次のアクションのID
	int nextActionId_;

	// 次のアクションのIDを設定する
	void SetNextActionId(void);

	// アクションデータを取得する
	void LoadJsonData(void);

	// アクションの更新処理を設定する
	void SetUpdateFunc(Action action);
};