#include <DxLib.h>
#include "../../Manager/KeyConfig.h"
#include "Player.h"

Player::Player(int _index)
	: index_(_index)
{
}

Player::~Player(void)
{
}

void Player::Load(void)
{
}

void Player::Init(void)
{
	//初期化
	moveSpeed_ = MOVE_SPEED;
}

void Player::Update(void)
{
	//移動処理
	Move();
}

void Player::Draw(void)
{
	DrawSphere3D(trans_.pos, 10.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	//座標
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%f,%f,%f", trans_.pos.x, trans_.pos.y, trans_.pos.z);
}

void Player::OnHit(const Collider* _hitCol)
{
}

void Player::Move(void)
{
	//入力
	auto& key = KeyConfig::GetInstance();

	//移動
	if(key.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT))
	{
		trans_.pos.x -= moveSpeed_;
	}
	if(key.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT))
	{
		trans_.pos.x += moveSpeed_;
	}
	if(key.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP))
	{
		trans_.pos.y -= moveSpeed_;
	}
	if(key.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN))
	{
		trans_.pos.y += moveSpeed_;
	}
}