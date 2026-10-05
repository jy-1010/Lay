#include "../../Application.h"
#include "JsonResource.h"
#include "ImageResource.h"
#include "ImageArrayResource.h"
#include "ModelResource.h"
#include "Sound/Sound2DResource.h"
#include "Sound/Sound3DResource.h"
#include "ShaderResource.h"
#include "FontResource.h"
#include "Resource.h"

Resource::Resource(nlohmann::json json)	: json_(json)
{
	LoadResourceInfo();
}

void Resource::LoadResourceInfo(void)
{
	path_ =Application::PATH_DATA + json_["Path"].get<std::string>();
}

bool Resource::IsPreLoad(void) const
{
	return json_["Preload"].get<bool>();
}

std::shared_ptr<JsonResource> Resource::CastToJsonResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<JsonResource> jsonResource = std::dynamic_pointer_cast<JsonResource>(resource);
	return jsonResource;
}

std::shared_ptr<ImageResource> Resource::CastToImageResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<ImageResource> imageResource = std::dynamic_pointer_cast<ImageResource>(resource);
	return imageResource;
}

std::shared_ptr<ImageArrayResource> Resource::CastToImageArrayResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<ImageArrayResource> imageArrayResource = std::dynamic_pointer_cast<ImageArrayResource>(resource);
	return imageArrayResource;
}

std::shared_ptr<ModelResource> Resource::CastToModelResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<ModelResource> modelResource = std::dynamic_pointer_cast<ModelResource>(resource);
	return modelResource;
}

std::shared_ptr<Sound2DResource> Resource::CastToSound2DResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<Sound2DResource> sound2DResource = std::dynamic_pointer_cast<Sound2DResource>(resource);
	return sound2DResource;
}

std::shared_ptr<Sound3DResource> Resource::CastToSound3DResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<Sound3DResource> sound3DResource = std::dynamic_pointer_cast<Sound3DResource>(resource);
	return sound3DResource;
}

std::shared_ptr<ShaderResource> Resource::CastToShaderResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<ShaderResource> shaderResource = std::dynamic_pointer_cast<ShaderResource>(resource);
	return shaderResource;
}

std::shared_ptr<FontResource> Resource::CastToFontResource(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<FontResource> fontResource = std::dynamic_pointer_cast<FontResource>(resource);
	return fontResource;
}
