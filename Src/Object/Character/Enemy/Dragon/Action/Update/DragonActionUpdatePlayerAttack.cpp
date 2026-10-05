#include "../../../../../../Utility/Utility.h"
#include "../../../../../../Manager/SceneManager.h"
#include "../DragonAction.h"
#include "DragonActionUpdatePlayerAttack.h"

DragonActionUpdatePlayerAttack::DragonActionUpdatePlayerAttack(std::weak_ptr<Transform> dragonTrans, nlohmann::json json, DragonAction& parent):
    DragonActionUpdateBase(dragonTrans,json,parent)
{
	time_ = 0.0f;
	Load();
}

DragonActionUpdatePlayerAttack::~DragonActionUpdatePlayerAttack(void)
{
}

void DragonActionUpdatePlayerAttack::Init(void)
{
	destinationPos_ = parent_.GetPlayerPos();
	destinationPos_.y += offsetHeight_;
	moveDir_ = VNorm(VSub(destinationPos_, dragonTrans_.lock()->pos));
	float rad = std::atan2f(moveDir_.x, moveDir_.z);
	rad = rad < 0 ? rad + Utility::Deg2RadF(360.0f) : rad;
	dragonTrans_.lock()->rot.y = rad;
	isDestination_ = false;
}

void DragonActionUpdatePlayerAttack::Update(void)
{
	VECTOR pos = dragonTrans_.lock()->pos;
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	float movePow = moveSpeed_ * deltaTime * parent_.GetBlockSize();
	time_ -= deltaTime;
	if (isDestination_)
	{
		//目的地到着後処理
		if (IsMoveLimit())
		{
			CalcNextDir();
			time_ = reChangeDelay_;
		}
	}
	else
	{
		//目的地到着前処理
		VECTOR sub = VSub(destinationPos_, pos);
		//移動距離より目的地が近い場合は到着させる
		if (sub.x * sub.x + sub.y * sub.y + sub.z * sub.z <= movePow * movePow)
		{
			dragonTrans_.lock()->pos = destinationPos_;
			isDestination_ = true;
			// rotからQuaternionを作成
			Quaternion q = Quaternion::Euler(dragonTrans_.lock()->rot);

			// モデルの前方向を取得
			VECTOR dir = q.GetForward();
			VECTOR move = VScale(dir, moveSpeed_ * deltaTime * parent_.GetBlockSize());
			if (dragonTrans_.lock()->pos.y < height_ * parent_.GetBlockSize())
			{
				//目標高さより低い場合
				move.y = riseSpeed_ * deltaTime * parent_.GetBlockSize();
			}
			moveDir_ = Utility::VNormalize(move);
			return;
		}
	}
	//移動処理
	Move();
}

void DragonActionUpdatePlayerAttack::Load(void)
{
	moveSpeed_ = json_["MoveSpeed"];
	riseSpeed_ = json_["RiseSpeed"];
	height_ = json_["Height"];
	moveRadius_ = json_["MoveRadius"];
	minChangeDir_ = json_["ChangeDIR"]["Min"];
	maxChangeDir_ = json_["ChangeDIR"]["Max"];
	reChangeDelay_ = json_["ReChangeDelay"];
	offsetHeight_ = json_["OffSetHeight"];
}

bool DragonActionUpdatePlayerAttack::IsMoveLimit(void)
{
	VECTOR pos = dragonTrans_.lock()->pos;
	float moveLimit = moveRadius_ * parent_.GetBlockSize();
	if (pos.x * pos.x + pos.z * pos.z >= moveLimit * moveLimit)
	{
		return true;
	}
	return false;
}

void DragonActionUpdatePlayerAttack::CalcNextDir(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	//ランダムに移動方向を決める
	int randDir = GetRand(maxChangeDir_ - minChangeDir_) + minChangeDir_;
	VECTOR pos = dragonTrans_.lock()->pos;
	//中心への角度を求める
	float rad = std::atan2f(-pos.x, -pos.z);
	//rad += DX_PI_F / 2.0f;
	rad = rad < 0 ? rad + Utility::Deg2RadF(360.0f) : rad;
	rad -= Utility::Deg2RadF(static_cast<float>(randDir));
	dragonTrans_.lock()->rot.y = rad;
	// rotからQuaternionを作成
	Quaternion q = Quaternion::Euler(dragonTrans_.lock()->rot);

	// モデルの前方向を取得
	VECTOR dir = q.GetForward();
	VECTOR move = VScale(dir, moveSpeed_ * deltaTime * parent_.GetBlockSize());
	if (dragonTrans_.lock()->pos.y < height_ * parent_.GetBlockSize())
	{
		//目標高さより低い場合
		move.y = riseSpeed_ * deltaTime * parent_.GetBlockSize();
	}
	moveDir_ = Utility::VNormalize(move);

}

void DragonActionUpdatePlayerAttack::Move(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	float movePow = moveSpeed_ * deltaTime * parent_.GetBlockSize();

	dragonTrans_.lock()->pos = VAdd(
		dragonTrans_.lock()->pos, VScale(moveDir_,movePow));
}
