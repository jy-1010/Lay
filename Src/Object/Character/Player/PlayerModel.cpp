#include <fstream>
#include "../../../Application.h"
#include "../../../Utility/Utility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource/JsonResource.h"
#include "../../../Manager/Resource/ShaderResource.h"
#include "../../Vertex/VertexInfo.h"
#include "Animation/PlayerAnimationManager.h"
#include "Skin/SkinManager.h"
#include "Skin/Skin.h"
#include "PlayerModel.h"

PlayerModel::PlayerModel(std::string skinName, nlohmann::json& params) : CharacterModelBase(params)
{
	LoadModelInfo();
	SetRendererInfo();
	MakePokygonInfo();
	LoadSkin(skinName);
	animationManager_ = std::make_unique<PlayerAnimationManager>(modelInfos_,animationKey_, rootPartName_);
	animationManager_->ApplyAnimation();
	polygonInfo_ = animationManager_->GetPolygonInfo();
}

PlayerModel::~PlayerModel(void)
{
}

void PlayerModel::Init(void)
{
}

void PlayerModel::Update(void)
{
	animationManager_->Update();
}

void PlayerModel::Draw(void)
{
	renderer_->Draw();
}

void PlayerModel::UIDraw(void)
{
}

void PlayerModel::SetSkinHandle(int handle)
{
	skinHandle_ = handle;
	material_->SetTextureBuf(0, skinHandle_);
}

void PlayerModel::SetAnimation(std::string animName, bool isCompulsion)
{
	if (isCompulsion)
	{
		animationManager_->Init(animName);
	}
	animationManager_->SetAnimation(animName);
}

void PlayerModel::ApplayVertexPos(VECTOR pos, VECTOR rot,float scale)
{
	animationManager_->ApplyAnimation();
	polygonInfo_ = animationManager_->GetPolygonInfo();
	ApplyScale(scale);
	ApplyRotation(rot);
	ApplyPosition(pos);
}

bool PlayerModel::IsAnimIdle(void) const
{
	auto& current = animationManager_->GetCurrentAnimationNames();
	for (auto& name : current)
	{
		if (name == "Idle")
		{
			return true;
		}
	}
	return false;
}

bool PlayerModel::IsPlayAnim(std::string name) const
{
	auto& current = animationManager_->GetCurrentAnimationNames();
	for (auto& currentname : current)
	{
		if (currentname == name)
		{
			return true;
		}
	}
	return false;
}

void PlayerModel::LoadModelInfo(void)
{
	pixelNum_ = params_["PixelNum"];
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(params_["Model"].get<std::string>()).lock();
	modelInfo_ = jsonResource->GetData();
	auto& textureSize = modelInfo_["TextureSize"];
	animationKey_ = modelInfo_["AnimationTemplate"].get<std::string>();
	textureSize_ = FloatVector2(textureSize["x"], textureSize["y"]);
	shaderInfo_.VSKey = modelInfo_["Shader"]["VS"];
	shaderInfo_.PSKey = modelInfo_["Shader"]["PS"];
	rootPartName_ = modelInfo_["RootPart"];
	for (auto& param : modelInfo_["Parts"])
	{
		Model_Part part;
		part.name = param["Name"];
		//if (part.name != "RightArm")
		//{
		//	continue;
		//}
		part.parentName = param["Parent"];
		part.affectParent.isRot = param["Affect"]["IsRot"];
		part.affectParent.isPos = param["Affect"]["IsPos"];
		auto& pivot = param["Pivot"];
		part.pivot = IntVector3{ pivot["x"], pivot["y"], pivot["z"] };
		for (auto& cubes : param["Cubes"])
		{
			Cube cube;
			auto& offset = cubes["Offset"];
			cube.offset = IntVector3{ offset["x"], offset["y"], offset["z"] };
			auto& pixelSize = cubes["PixelSize"];
			cube.pixelSize = IntVector3{ pixelSize["x"], pixelSize["y"], pixelSize["z"] };
			auto& uvOffset = cubes["UV"];
			cube.uvOffset = FloatVector2(uvOffset["x"] / textureSize_.u, uvOffset["y"] / textureSize_.v);
			part.cubes.push_back(cube);
		}
		modelParts_.push_back(part);
	}
}

void PlayerModel::LoadSkin(std::string skinName)
{
	auto& skinManager = SkinManager::GetInstance();
	skinHandle_ = skinManager.GetHandleId(skinName);
	material_->AddTextureBuf(skinHandle_);
}

void PlayerModel::MakePokygonInfo(void)
{
	polygonInfo_.clear();
	for (auto& part : modelParts_)
	{
		for (auto& cube : part.cubes)
		{
			VertexInfo::Cube_Param param;
			param.key = modelInfo_["RenderTemplate"].get<std::string>();
			param.center = (part.pivot + cube.offset).ToVECTOR();
			param.cubeSize = cube.pixelSize.ToVECTOR();
			param.TextureSize = textureSize_;
			param.startUV = cube.uvOffset;
			auto cubePolygonInfo = VertexInfo::LoadFromFile(param);
			part.pivot *= VertexInfo::GetPixelSize(param.key);
			ModelInfo modelInfo;
			modelInfo.part = part;
			modelInfo.polygonInfo = cubePolygonInfo;
			modelInfos_.push_back(modelInfo);
		}
	}
}

void PlayerModel::SetRendererInfo(void)
{
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	auto shaderVSResource = resourceManager.GetShaderResource(shaderInfo_.VSKey).lock();
	auto shaderPSResource = resourceManager.GetShaderResource(shaderInfo_.PSKey).lock();
	material_ = std::make_unique<Polygon3DMaterial>(shaderVSResource->GetHandleId(), 1, shaderPSResource->GetHandleId(), 1);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
	material_->AddConstBufPS(FLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void PlayerModel::ApplyPosition(VECTOR pos)
{
	for (auto& polygon : polygonInfo_.vertex)
	{
		polygon.pos =VAdd(polygon.pos, pos);
	}
}

void PlayerModel::ApplyRotation(VECTOR rot)
{
	for (auto& polygon : polygonInfo_.vertex)
	{
		polygon.pos = Utility::RotateX(polygon.pos, rot.x);
		polygon.pos = Utility::RotateY(polygon.pos, rot.y);
		polygon.pos = Utility::RotateZ(polygon.pos, rot.z);
	}
}

void PlayerModel::ApplyScale(float scale)
{
	for (auto& polygon : polygonInfo_.vertex)
	{
		polygon.pos = VScale(polygon.pos,scale);
	}
}
