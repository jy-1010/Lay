#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Resource/JsonResource.h"
#include "../../../Player/Player.h"
#include "../Breath/DragonBreath.h"
#include "DragonAction.h"
#include "DragonActionManager.h"

DragonActionManager::DragonActionManager(std::string key,std::shared_ptr<Transform> transform, const Player& player,float blockSize):player_(player)
{
	blockSize_ = blockSize;
	key_ = key;
	transform_ = transform;
	breath_ = std::make_unique<DragonBreath>();
	actionData_.clear();
	LoadJsonData();
}

DragonActionManager::~DragonActionManager(void)
{
}

void DragonActionManager::Init(void)
{
}

void DragonActionManager::Update(void)
{
	auto& currentAction = actions_[currentActionId_];
	currentAction->Update();
	if (currentAction->IsEnd())
	{
		//アクションを変える
		currentActionId_ = currentAction->GetNextActionId();
		actions_[currentActionId_]->Init();
		actionData_.push_back(currentActionId_);
	}
}

void DragonActionManager::Draw(void)
{
}

void DragonActionManager::UIDraw(void)
{
	//行動パターンの描画
	const int OFFSET = 10;
	const int SIZE_Y = 20;
	int size = static_cast<int>(actionData_.size());
	for (int i = 0; i < size; i++)
	{
		DrawString(OFFSET, i * SIZE_Y + OFFSET, actions_[actionData_[i]]->GetName().c_str(), 0xffffff);
	}
}

void DragonActionManager::CreateBreath(void)
{
	breath_->CreateBreath(transform_.lock()->pos,GetPlayerPos());
}

const VECTOR DragonActionManager::GetPlayerPos(void) const
{
	return  player_.GetTransform().lock()->pos;
}

void DragonActionManager::LoadJsonData(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(key_).lock();
	auto& json = jsonResource->GetData();
	currentActionId_ = json["StartId"];
	actionData_.push_back(currentActionId_);
	for (auto& pattern : json["Pattern"])
	{
		std::shared_ptr<DragonAction> action = std::make_shared<DragonAction>(pattern,transform_.lock(),blockSize_,*this);
		actions_.emplace(action->GetId(), action);
	}
}
