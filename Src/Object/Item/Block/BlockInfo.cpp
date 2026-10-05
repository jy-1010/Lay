#include <fstream>
#include "../../../Application.h"
#include "../../../Utility/JsonUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource/JsonResource.h"
#include "../../../Manager/Resource/ImageResource.h"
#include "../../Vertex/VertexInfo.h"
#include "BlockInfo.h"

BlockInfo::BlockInfo(void)
{
	auto& resManager = ResourceManager::GetInstance();
	blocks = resManager.GetJsonResource("BlocksJson").lock()->GetData();

	shaderInfo_.VSKey = blocks["Shader"]["VS"];
	shaderInfo_.PSKey = blocks["Shader"]["PS"];

	textureSize_ = JsonUtility::GetPosToFloat2D(blocks["TextureSize"]);

	imageHandle_ = resManager.GetImageResource(blocks["Image"]).lock()->GetHandleId();

	std::vector<std::string> blockNames = blocks["Names"].get<std::vector<std::string>>();
	for (auto& blockName : blockNames)
	{
		LoadBlockInfo(blockName);
	}
}

BlockInfo::~BlockInfo(void)
{
}

void BlockInfo::Init(void)
{
}

void BlockInfo::Update(void)
{
}

void BlockInfo::Draw(void)
{
}

void BlockInfo::UIDraw(void)
{
}

const std::map<std::string, unsigned short> BlockInfo::GetPairNameAndID(void)
{
	std::map<std::string, unsigned short>ret;
	for (auto& param : params_)
	{
		ret.emplace(param.second.name, param.first);
	}

	return ret;
}

const BlockInfo::Param& BlockInfo::GetParam(std::string name)
{
	for (auto& param : params_)
	{
		if (param.second.name == name)
		{
			return param.second;
		}
	}
}

const int BlockInfo::GetSize(void)
{
	return blocks["1SquareSize"] * VertexInfo::GetPixelSize(blocks["RenderTemplate"]);;
}

void BlockInfo::LoadBlockInfo(const std::string blockName)
{
	Param param;
	param.name = blockName;
	param.id = blocks[blockName]["ID"].get<unsigned short>();
	param.hardness = blocks[blockName]["Hardness"].get<float>();
	param.luminous = blocks[blockName]["Luminous"].get<float>();
	param.toolType = blocks[blockName]["ToolType"].get<std::string>();
	param.toolLevel = blocks[blockName]["ToolLevel"].get<std::vector<std::string>>();
	param.isCorrectAnotherTool = blocks[blockName]["IsCorrectAnotherTool"].get<bool>();
	param.uvOffset.u = blocks[blockName]["UVOffset"]["u"].get<float>() / textureSize_.x;
	param.uvOffset.v = blocks[blockName]["UVOffset"]["v"].get<float>() / textureSize_.y;
	param.fasesPolygonInfo = MakePolygon(param);
	params_.emplace(param.id, param);
}

std::map<std::string, Polygon3DRenderer::PolygonInfo> BlockInfo::MakePolygon(Param param)
{
	int cubeSize = blocks["1SquareSize"];
	VertexInfo::Cube_Param cubeParam;
	cubeParam.center = { static_cast<float>(cubeSize) / 2,static_cast<float>(cubeSize) / 2,static_cast<float>(cubeSize) / 2 };
	cubeParam.cubeSize = IntVector3(cubeSize, cubeSize, cubeSize).ToVECTOR();
	cubeParam.key = blocks["RenderTemplate"].get<std::string>();
	cubeParam.startUV = param.uvOffset;
	cubeParam.TextureSize = JsonUtility::GetPosToFloat2D(blocks["TextureSize"]);
	return VertexInfo::LoadFromFileAFace(cubeParam);
}
