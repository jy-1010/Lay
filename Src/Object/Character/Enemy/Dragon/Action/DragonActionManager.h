#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "../../../../Common/Transform.h"

class DragonAction;
class DragonBreath;
class Player;

class DragonActionManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">アクションのリソースキー</param>
	/// <param name="transform">ドラゴンのトランスフォーム</param>
	/// <param name="player">プレイヤー情報</param>
	DragonActionManager(std::string key,std::shared_ptr<Transform> transform, const Player& player,float blockSizer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonActionManager(void);

	/// <summary>
	/// 初期化
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
	/// ブレスを作成する
	/// </summary>
	/// <param name=""></param>
	void CreateBreath(void);

	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const VECTOR GetPlayerPos(void)const;

private:

	float blockSize_;

	//ドラゴンの情報
	std::weak_ptr<Transform> transform_;

	//プレイヤー
	const Player& player_;

	// アクションの情報jsonのキー
	std::string key_;

	//アクションのリスト
	std::map<int,std::shared_ptr<DragonAction>> actions_;

	//ブレス
	std::unique_ptr<DragonBreath> breath_;

	//現在のアクションのID
	int currentActionId_;

	std::vector<int> actionData_;

	void LoadJsonData(void);
};

