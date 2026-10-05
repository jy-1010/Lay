#include "../../../../../Manager/SceneManager.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Resource/JsonResource.h"
#include "DragonActionManager.h"
#include "Update/DragonActionUpdateBase.h"
#include "Update/DragonActionUpdateFlyMove.h"
#include "Update/DragonActionUpdateLanding.h"
#include "Update/DragonActionUpdateTakeFlight.h"
#include "Update/DragonActionUpdatePlayerAttack.h"
#include "DragonAction.h"

DragonAction::DragonAction(nlohmann::json jsonData, std::shared_ptr<Transform> transform,float blockSize,const DragonActionManager& parent):parent_(parent)
{
	sumWeight_ = 0;
	transform_ = transform;
	time_  = 0.0f;
	jsonData_ = jsonData;
	blockSize_ = blockSize;
	LoadJsonData();
	Init();
}

DragonAction::~DragonAction(void)
{
}

void DragonAction::Init(void)
{
	time_ = 0.0f;
	if (update_)
	{
		update_->Init();
	}
	SetNextActionId();
}

void DragonAction::Update(void)
{
	update_->Update();
	time_ += SceneManager::GetInstance().GetDeltaTime();
}

void DragonAction::Draw(void)
{
}

void DragonAction::UIDraw(void)
{
}

const VECTOR DragonAction::GetPlayerPos(void) const
{
	return parent_.GetPlayerPos();
}

void DragonAction::SetNextActionId(void)
{
	//重みづけされた中から次の行動を決める
	int randomValue = rand() % sumWeight_;
	int accumulatedWeight = 0;
	for (const auto& nextAction : nextActions_)
	{
		accumulatedWeight += nextAction.weight;
		if (randomValue < accumulatedWeight)
		{
			nextActionId_ = nextAction.id;
			break;
		}
	}
}

void DragonAction::LoadJsonData(void)
{
	// 自分の情報を取得する
	id_ = jsonData_["Id"];
	action_ = static_cast<Action>(id_);
	name_ = jsonData_["Name"];
	duration_ = jsonData_["Duration"];
	SetUpdateFunc(action_);
	for (auto& nextActionJson : jsonData_["Next"])
	{
		// 次のアクションの情報を取得する
		NextActionInfo nextActionInfo;
		nextActionInfo.id = nextActionJson["Id"];
		nextActionInfo.weight = nextActionJson["Weight"];
		nextActionInfo.nextAction = static_cast<Action>(nextActionInfo.id);
		sumWeight_ += nextActionInfo.weight;
		nextActions_.push_back(nextActionInfo);
	}
}

void DragonAction::SetUpdateFunc(Action action)
{
	auto& resourceManager = ResourceManager::GetInstance();
	switch (action)
	{
	case DragonAction::Action::NONE:
		break;
	case DragonAction::Action::FLY_MOVE:
		update_ = std::make_unique<DragonActionUpdateFlyMove>(transform_, resourceManager.GetJsonResource(jsonData_["Json"]).lock()->GetData(), *this);
		break;
	case DragonAction::Action::FLY_BREATH_ATTACK:
		//update_ = std::make_unique<DragonActionUpdateLanding>(transform_, resourceManager.GetJsonResource(jsonData_["Json"]).lock()->GetData(), *this);
		break;
	case DragonAction::Action::LANDING:
		update_ = std::make_unique<DragonActionUpdateLanding>(transform_, resourceManager.GetJsonResource(jsonData_["Json"]).lock()->GetData(), *this);
		break;
	case DragonAction::Action::LAND_ATTACK:
		//update_ = std::make_unique<DragonActionUpdateLanding>(transform_, resourceManager.GetJsonResource(jsonData_["Json"]).lock()->GetData(), *this);
		break;
	case DragonAction::Action::TAKE_FLIGHT:
		update_ = std::make_unique<DragonActionUpdateTakeFlight>(transform_, resourceManager.GetJsonResource(jsonData_["Json"]).lock()->GetData(), *this);
		break;
	case DragonAction::Action::FLY_PLAYER_ATTACK:
		update_ = std::make_unique<DragonActionUpdatePlayerAttack>(transform_, resourceManager.GetJsonResource(jsonData_["Json"]).lock()->GetData(), *this);
		break;
	default:
		break;
	}
}
