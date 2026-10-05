#include "../../../../../../Utility/JsonUtility.h"
#include "../../../../../../Utility/Utility.h"
#include "../../../../../../Manager/SceneManager.h"
#include "../DragonAction.h"
#include "DragonActionUpdateLanding.h"

DragonActionUpdateLanding::DragonActionUpdateLanding(std::weak_ptr<Transform> dragonTrans, nlohmann::json json, DragonAction& parent)
	:DragonActionUpdateBase(dragonTrans,json,parent)
{
	Load();
	Init();
}

DragonActionUpdateLanding::~DragonActionUpdateLanding(void)
{
}

void DragonActionUpdateLanding::Init(void)
{
	VECTOR pos = dragonTrans_.lock()->pos;
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	float movePow = moveSpeed_ * deltaTime * parent_.GetBlockSize();
	VECTOR sub = VSub(destinationPos_, pos);
	//移動距離より目的地が近い場合は到着させる
	if (sub.x * sub.x + sub.y * sub.y + sub.z * sub.z <= movePow * movePow)
	{
		return;
	}
	CalcNextDir();
}

void DragonActionUpdateLanding::Update(void)
{
	Move();
}

void DragonActionUpdateLanding::Load(void)
{
	destinationMapPos_ = JsonUtility::GetPosToInt3D(json_["DestinationMapPos"]);
	moveSpeed_ = json_["MoveSpeed"];
	destinationPos_ = (destinationMapPos_ * parent_.GetBlockSize()).ToVECTOR();
}

void DragonActionUpdateLanding::CalcNextDir(void)
{
	VECTOR pos = dragonTrans_.lock()->pos;
	moveDir_ = VSub(destinationPos_, pos);
	moveDir_ = Utility::VNormalize(moveDir_);
	//中心への角度を求める
	float rad = std::atan2f(moveDir_.x, moveDir_.z);
	rad = rad < 0 ? rad + Utility::Deg2RadF(360.0f) : rad;
	dragonTrans_.lock()->rot.y = rad;
}

void DragonActionUpdateLanding::Move(void)
{
	VECTOR pos = dragonTrans_.lock()->pos;
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	float movePow = moveSpeed_ * deltaTime * parent_.GetBlockSize();
	VECTOR sub = VSub(destinationPos_, pos);
	//移動距離より目的地が近い場合は到着させる
	if (sub.x * sub.x + sub.y * sub.y + sub.z * sub.z <= movePow * movePow)
	{
		dragonTrans_.lock()->pos = destinationPos_;
		return;
	}
	dragonTrans_.lock()->pos = VAdd(pos, VScale(moveDir_, movePow));
}
