#pragma once
#include <vector>
#include <memory>
#include "../../Common/Singleton.h"

class Player;

class PlayerManager : public Singleton<PlayerManager>
{
	//シングルトンのフレンド宣言
	friend class Singleton<PlayerManager>;

public:

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//プレイヤーの追加
	void AddPlayer(void);

	//プレイヤーの削除
	void DeletePlayer(const int _index);

	//プレイヤーの取得
	std::unique_ptr<Player>& GetPlayer(const int _index) { return players_[_index]; }
	std::vector<std::unique_ptr<Player>>& GetPlayers(void) { return players_; }

private:

	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;

	// コンストラクタ
	PlayerManager(void);

	// デストラクタ
	~PlayerManager(void);

	//コピーコンストラクタを禁止
	PlayerManager(const PlayerManager&) = delete;
	PlayerManager& operator=(const PlayerManager&) = delete;

	//読み込み
	void Load(void)override;
};