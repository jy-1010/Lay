#include "../../../../../../Utility/Utility.h"
#include "../../../../../../Manager/SceneManager.h"
#include "../DragonAction.h"
#include "DragonActionUpdateTakeFlight.h"

DragonActionUpdateTakeFlight::DragonActionUpdateTakeFlight(std::weak_ptr<Transform> dragonTrans, nlohmann::json json, DragonAction& parent):
	DragonActionUpdateBase(dragonTrans,json,parent)
{
	time_ = 0.0f;
	Load();
}

DragonActionUpdateTakeFlight::~DragonActionUpdateTakeFlight(void)
{
}

void DragonActionUpdateTakeFlight::Init(void)
{
}

void DragonActionUpdateTakeFlight::Update(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	time_ -= deltaTime;
	//移動処理
	Move();
	if ((IsMoveLimit() /*&& time_ < 0.0f*/))
	{
		CalcNextDir();
		time_ = reChangeDelay_;
	}
}

void DragonActionUpdateTakeFlight::Load(void)
{
	moveSpeed_ = json_["MoveSpeed"];
	riseSpeed_ = json_["RiseSpeed"];
	height_ = json_["Height"];
	moveRadius_ = json_["MoveRadius"];
	minChangeDir_ = json_["ChangeDIR"]["Min"];
	maxChangeDir_ = json_["ChangeDIR"]["Max"];
	reChangeDelay_ = json_["ReChangeDelay"];
}

bool DragonActionUpdateTakeFlight::IsMoveLimit(void)
{
	VECTOR pos = dragonTrans_.lock()->pos;
	float moveLimit = moveRadius_ * parent_.GetBlockSize();
	if (pos.x * pos.x + pos.z * pos.z >= moveLimit * moveLimit)
	{
		return true;
	}
	return false;
}

void DragonActionUpdateTakeFlight::CalcNextDir(void)
{
	//ランダムに移動方向を決める
	int randDir = GetRand(maxChangeDir_ - minChangeDir_) + minChangeDir_;
	VECTOR pos = dragonTrans_.lock()->pos;
	//中心への角度を求める
	float rad = std::atan2f(-pos.x, -pos.z);
	//rad += DX_PI_F / 2.0f;
	rad = rad < 0 ? rad + Utility::Deg2RadF(360.0f) : rad;
	rad -= Utility::Deg2RadF(static_cast<float>(randDir));
	dragonTrans_.lock()->rot.y = rad;
}

void DragonActionUpdateTakeFlight::Move(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

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

	dragonTrans_.lock()->pos = VAdd(
		dragonTrans_.lock()->pos, move);
}
