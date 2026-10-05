#include "../../../../Application.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource/JsonResource.h"
#include "PlayerAnimation.h"
#include "PlayerAnimationManager.h"

PlayerAnimationManager::PlayerAnimationManager(const std::vector<PlayerModel::ModelInfo>& modelInfos, std::string key, std::string rootPartName) :modelInfos_(modelInfos), rootPartName_(rootPartName)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(key).lock();
	animationInfo_ = jsonResource->GetData();
	std::string rootPath = animationInfo_["RootPath"];
	for (auto& animation : animationInfo_["Animations"])
	{
		std::string name = animation["Name"];
		std::string path = Application::PATH_DATA + rootPath + animation["Path"].get<std::string>();
		AddAnimation(name, std::make_shared<PlayerAnimation>(path));
	}
	defaultAnimationName_ = animationInfo_["DefaultAnimation"].get<std::string>();
	SetAnimation(defaultAnimationName_);
}

void PlayerAnimationManager::Init(void)
{
	for (auto& animationName : animationNames)
	{
		animations[animationName]->Init();
	}
	animationNames.clear();
	SetAnimation(defaultAnimationName_);
}

void PlayerAnimationManager::Init(std::string name)
{
	std::vector<std::string> eraseNames;
	int rayer = animations[name]->GetLayer();
	for (auto& animationName : animationNames)
	{
		auto& animation = animations[animationName];
		if (rayer != animation->GetLayer())
		{
			continue;
		}
		eraseNames.push_back(animationName);
		animation->Init();
	}
	for (auto& eraseName : eraseNames)
	{
		auto iter = std::find(animationNames.begin(), animationNames.end(), eraseName);
		if (iter != animationNames.end())
		{
			animationNames.erase(iter);
		}
	}
	for (auto& animationName : animationNames)
	{
		auto& animation = animations[animationName];
		if (animations[defaultAnimationName_]->GetLayer() == animation->GetLayer())
		{
			return;
		}
	}
	SetAnimation(defaultAnimationName_);
}

void PlayerAnimationManager::Update(void)
{
	std::vector<std::string> eraseNames;
	for(auto& animationName : animationNames)
	{
		if (animations[animationName]->IsEnd())
		{
			animations[animationName]->Init();
			eraseNames.push_back(animationName);
			continue;
		}
		animations[animationName]->Update();
	}
	for (auto& eraseName : eraseNames)
	{
		auto iter = std::find(animationNames.begin(), animationNames.end(), eraseName);
		if (iter != animationNames.end())
		{
			animationNames.erase(iter);
		}
	}
}

void PlayerAnimationManager::SetAnimation(std::string name)
{
	if (name == "Dash")
	{
		int i = 0;
	}
	auto iter = std::find(animationNames.begin(), animationNames.end(), name);
	if (iter != animationNames.end())
	{
		//すでにアニメーションがセットされている場合は何もしない
		return;
	}
	std::vector<std::string> eraseNames;
	int i = 0;
	for (auto& animationName : animationNames)
	{
		i++;
		if (animationName == "")
		{
			continue;
		}
		//同じレイヤーかつ優先度が同じか低いアニメーションを停止する
		if (animations[name]->GetPriority() <= animations[animationName]->GetPriority()&&
			animations[name]->GetLayer() == animations[animationName]->GetLayer())
		{
			return;
		}
		else if (animations[name]->GetLayer() == animations[animationName]->GetLayer())
		{
			eraseNames.push_back(animationName);
		}
	}
	for (auto& eraseName : eraseNames)
	{
		auto iter = std::find(animationNames.begin(), animationNames.end(), eraseName);
		if (iter != animationNames.end())
		{
			animationNames.erase(iter);
		}
	}
	animationNames.push_back(name);
}

void PlayerAnimationManager::StopAnimation(std::string name)
{
	auto iter = std::find(animationNames.begin(), animationNames.end(), name);
	if (iter != animationNames.end())
	{
		animations[name]->Init();
		animationNames.erase(iter);
	}
	if (animationNames.size() == 0 && name != defaultAnimationName_)
	{
		SetAnimation(defaultAnimationName_);
	}
}

Polygon3DRenderer::PolygonInfo PlayerAnimationManager::GetPolygonInfo(void) const
{
	return polygonInfo_;
}

void PlayerAnimationManager::AddAnimation(std::string name, std::shared_ptr<PlayerAnimation> animation)
{
	animations[name] = animation;
}

void PlayerAnimationManager::ApplyAnimation(void)
{
	int maxLayer = 0;
	std::map<int, std::vector<std::weak_ptr<PlayerAnimation>>> layerAnimations;
	//レイヤーごとにアニメーションを分ける
	for (auto& animationName : animationNames)
	{
		auto& animation = animations[animationName];
		int layer = animation->GetLayer();
		if (layer > maxLayer)
		{
			maxLayer = layer;
		}
		layerAnimations[layer].push_back(animation);
	}
	//レイヤーの高い順にアニメーションを適用する
	auto modelInfos = modelInfos_;
	std::vector<std::string> appliedMask;
	for (int layer = maxLayer; layer >= 0; layer--)
	{
		for (auto& animation : layerAnimations[layer])
		{
			if (auto anim = animation.lock())
			{
				modelInfos = anim->ApplyAnimation(modelInfos, appliedMask);
				auto mask = anim->GetMask();
				appliedMask.insert(appliedMask.end(), mask.begin(), mask.end());
			}
		}
	}
	//アニメーションを適用したモデルの情報からポリゴン情報を作成する
	polygonInfo_.clear();
	for (auto& modelInfo : modelInfos)
	{
		polygonInfo_.vertex.insert(polygonInfo_.vertex.end(), modelInfo.polygonInfo.vertex.begin(), modelInfo.polygonInfo.vertex.end());
		int indexOffset = static_cast<int>(polygonInfo_.vertex.size()) - static_cast<int>(modelInfo.polygonInfo.vertex.size());
		for (auto& index : modelInfo.polygonInfo.Indices)
		{
			polygonInfo_.Indices.push_back(index + indexOffset);
		}
	}

}
