#include <fstream>
#include "../Application.h"
#include "Resource/JsonResource.h"
#include "Resource/ImageResource.h"
#include "Resource/ImageArrayResource.h"
#include "Resource/ModelResource.h"
#include "Resource/Sound/Sound2DResource.h"
#include "Resource/Sound/Sound3DResource.h"
#include "Resource/ShaderResource.h"
#include "Resource/FontResource.h"
#include "ResourceManager.h"


ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	std::fstream jsonFile(Application::PATH_JSON + "Resource.json");
	nlohmann::json json;
	jsonFile >> json;
	for (auto& info : json["Infos"].items())
	{
		std::string key = info.key();
		std::string typeStr = info.value()["Type"].get<std::string>();
		GetResourceTypeFromString(typeStr);
		std::shared_ptr<Resource> resource = CreateResource(info.value(), GetResourceTypeFromString(typeStr));
		resourcesMap_.emplace(key, resource);
	}
}

void ResourceManager::Release(void)
{
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
	instance_ = nullptr;
}

void ResourceManager::Load(std::string key)
{
	if (resourcesMap_[key]->IsLoaded())
	{
		return;
	}
	resourcesMap_[key]->Load();
}

std::weak_ptr<JsonResource> ResourceManager::GetJsonResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::JSON)
	{
		return std::weak_ptr<JsonResource>();
	}
	return std::static_pointer_cast<JsonResource>(resource);
}

std::weak_ptr<ImageResource> ResourceManager::GetImageResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::IMAGE)
	{
		return std::weak_ptr<ImageResource>();
	}
	return std::static_pointer_cast<ImageResource>(resource);
}

std::weak_ptr<ImageArrayResource> ResourceManager::GetImageArrayResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::IMAGEARRAY)
	{
		return std::weak_ptr<ImageArrayResource>();
	}
	return std::static_pointer_cast<ImageArrayResource>(resource);
}

std::weak_ptr<ModelResource> ResourceManager::GetModelResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::MODEL)
	{
		return std::weak_ptr<ModelResource>();
	}
	return std::static_pointer_cast<ModelResource>(resource);
}

std::weak_ptr<Sound2DResource> ResourceManager::GetSound2DResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::SOUND_2D)
	{
		return std::weak_ptr<Sound2DResource>();
	}
	return std::static_pointer_cast<Sound2DResource>(resource);
}

std::weak_ptr<Sound3DResource> ResourceManager::GetSound3DResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::SOUND_3D)
	{
		return std::weak_ptr<Sound3DResource>();
	}
	return std::static_pointer_cast<Sound3DResource>(resource);
}

std::weak_ptr<ShaderResource> ResourceManager::GetShaderResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::SHADER)
	{
		return std::weak_ptr<ShaderResource>();
	}
	return std::static_pointer_cast<ShaderResource>(resource);
}

std::weak_ptr<FontResource> ResourceManager::GetFontResource(const std::string& key)
{
	auto resource = GetResource(key);
	if (resource == nullptr || resource->GetResourceType() != Resource::TYPE::FONT)
	{
		return std::weak_ptr<FontResource>();
	}
	return std::static_pointer_cast<FontResource>(resource);
}

Resource::TYPE ResourceManager::GetResourceTypeFromString(const std::string& str)
{
	if (str == "Json")
	{
		return Resource::TYPE::JSON;
	}
	else if(str == "Image")
	{
		return Resource::TYPE::IMAGE;
	}
	else if(str == "ImageArray")
	{
		return Resource::TYPE::IMAGEARRAY;
	}
	else if(str == "Model")
	{
		return Resource::TYPE::MODEL;
	}
	else if(str == "Sound2D")
	{
		return Resource::TYPE::SOUND_2D;
	}
	else if(str == "Sound3D")
	{
		return Resource::TYPE::SOUND_3D;
	}
	else if(str == "Shader")
	{
		return Resource::TYPE::SHADER;
	}
	else if(str == "Font")
	{
		return Resource::TYPE::FONT;
	}
	return Resource::TYPE::NONE;
}

std::shared_ptr<Resource> ResourceManager::CreateResource( const nlohmann::json& json, Resource::TYPE type)
{
	switch (type)
	{
	case Resource::TYPE::JSON:
		return std::make_shared<JsonResource>(json);
	case Resource::TYPE::IMAGE:
		return std::make_shared<ImageResource>(json);
	case Resource::TYPE::IMAGEARRAY:
		return std::make_shared<ImageArrayResource>(json);
	case Resource::TYPE::MODEL:
		return std::make_shared<ModelResource>(json);
	case Resource::TYPE::SOUND_2D:
		return std::make_shared<Sound2DResource>(json);
	case Resource::TYPE::SOUND_3D:
		return std::make_shared<Sound3DResource>(json);
	case Resource::TYPE::SHADER:
		return std::make_shared<ShaderResource>(json);
	case Resource::TYPE::FONT:
		return std::make_shared<FontResource>(json);
	}
	return nullptr;
}

std::shared_ptr<Resource> ResourceManager::GetResource(const std::string& key)
{
	auto it = resourcesMap_.find(key);
	if (it != resourcesMap_.end())
	{
		auto& resource = it->second;
		if (!resource->IsLoaded())
		{
			resource->Load();
		}
		return resource;
	}
	return nullptr;
}
