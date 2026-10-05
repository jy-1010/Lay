#include "ShaderResource.h"

ShaderResource::ShaderResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
	if(IsPreLoad())
	{
		Load();
	}
}

ShaderResource::~ShaderResource(void)
{
	DeleteShader(handleId_);
}

bool ShaderResource::Load(void)
{
	switch (shaderType_)
	{
	case SHADER_TYPE::VERTEX:
		handleId_ = LoadVertexShader(path_.c_str());
		break;
	case SHADER_TYPE::PIXEL:
		handleId_ = LoadPixelShader(path_.c_str());
		break;
	default:
		break;
	}
	return handleId_ != -1;
}

bool ShaderResource::IsLoaded(void) const
{
	return handleId_ != -1;
}

void ShaderResource::LoadResourceInfo(void)
{
	Init();
	std::string shaderTypeStr = json_["ShaderType"].get<std::string>();
	if (shaderTypeStr == "Vertex")
	{
		shaderType_ = SHADER_TYPE::VERTEX;
	}
	else if (shaderTypeStr == "Pixel")
	{
		shaderType_ = SHADER_TYPE::PIXEL;
	}
	else
	{
		shaderType_ = SHADER_TYPE::NONE;
	}
}

void ShaderResource::Init(void)
{
	handleId_ = -1;
	resourceType_ = TYPE::SHADER;
	shaderType_ = SHADER_TYPE::NONE;
}
