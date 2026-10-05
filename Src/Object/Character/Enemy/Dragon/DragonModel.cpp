#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource/JsonResource.h"
#include "../../../../Manager/Resource/ModelResource.h"
#include "../../../../Manager/Resource/ShaderResource.h"
#include "../../../Common/AnimationController.h"
#include "../../../Common/Transform.h"
#include "DragonModel.h"

DragonModel::DragonModel(std::shared_ptr<Transform> transform, nlohmann::json json) :CharacterModelBase(json), transform_(transform)
{
	LoadModelInfo();
	InitAnimation();
	SetRendererInfo();
}

DragonModel::~DragonModel(void)
{
}

void DragonModel::Init(void)
{
}

void DragonModel::Update(void)
{
	animationController_->Update();
	transform_.lock()->Update();
}

void DragonModel::Draw(void)
{
	renderer_->Draw();
}

void DragonModel::UIDraw(void)
{
}

void DragonModel::SetColorRate(FLOAT4 colorRate)
{
	material_->SetConstBufPS(0, colorRate);
}

void DragonModel::SetRendererInfo(void)
{
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	auto shaderVSResource = resourceManager.GetShaderResource(shaderInfo_.VSKey).lock();
	auto shaderPSResource = resourceManager.GetShaderResource(shaderInfo_.PSKey).lock();
	material_ = std::make_unique<ModelMaterial>(shaderVSResource->GetHandleId(), 1, shaderPSResource->GetHandleId(), 1);
	renderer_ = std::make_shared<ModelRenderer>(transform_.lock()->modelId, *material_);
	material_->AddConstBufPS(DEFAULT_COLOR_RATE);
}

void DragonModel::LoadModelInfo(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(params_["Model"].get<std::string>()).lock();
	modelInfo_ = jsonResource->GetData();
	animationKey_ = modelInfo_["AnimationTemplate"].get<std::string>();
	shaderInfo_.VSKey = modelInfo_["Shader"]["VS"];
	shaderInfo_.PSKey = modelInfo_["Shader"]["PS"];
	auto modelResource = resourceManager.GetModelResource(modelInfo_["Model"].get<std::string>()).lock();
	transform_.lock()->modelId = modelResource->GetHandleId();
}

void DragonModel::InitAnimation(void)
{
	animationController_ = std::make_unique<AnimationController>(transform_.lock()->modelId);
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(animationKey_).lock();
	auto& animationInfo = jsonResource->GetData();
	for (auto& anim : animationInfo["Infos"])
	{
		AnimationInfo info;
		info.id = anim["Id"];
		info.animName = anim["Name"];
		info.speed = anim["PlaySpeedFPS"];
		animationController_->Add(info.id, info.speed);
		animationInfos_.push_back(info);
	}
	animationController_->Play(GetAnimationIdByName("FlyForward"));
}

int DragonModel::GetAnimationIdByName(const std::string& name) const
{
	for (auto& info : animationInfos_)
	{
		if(info.animName == name)
		{
			return info.id;
		}
	}
	return -1;
}
