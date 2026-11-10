#include "Player.h"
#include "PlayerManager.h"

void PlayerManager::Init(void)
{
	players_.clear();
	AddPlayer();
}

void PlayerManager::Update(void)
{
	//プレイヤーの更新
	for (auto& player : players_)
	{
		player->Update();
	}
}

void PlayerManager::Draw(void)
{
	//プレイヤーの描画
	for (auto& player : players_)
	{
		player->Draw();
	}
}

void PlayerManager::AddPlayer(void)
{
	//プレイヤーの追加
	std::unique_ptr<Player> player = std::make_unique<Player>(players_.size());
	players_.push_back(std::move(player));
}

void PlayerManager::DeletePlayer(const int _index)
{
	//プレイヤーの削除
	players_.erase(players_.begin() + _index);
}

PlayerManager::PlayerManager(void)
{
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Load(void)
{
}