#include <fstream>
#include "JsonResource.h"

JsonResource::JsonResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
	if(IsPreLoad())
	{
		Load();
	}
}

JsonResource::~JsonResource(void)
{
}

bool JsonResource::Load(void)
{
	jsonFile_ = std::fstream(path_,std::ios::in|std::ios::out);
	if (!jsonFile_.is_open())
	{
		isLoaded_ = false;
		return false;
	}

	jsonFile_ >> data_;

	jsonFile_.close();

	isLoaded_ = true;
	return true;
}

bool JsonResource::IsLoaded(void) const
{
	return isLoaded_;
}

void JsonResource::OutFile(nlohmann::json data)
{
	jsonFile_.open(path_, std::ios::out | std::ios::trunc);
	jsonFile_ << data.dump(4);
	jsonFile_.close();
}

void JsonResource::LoadResourceInfo(void)
{
	Init();
}

void JsonResource::Init(void)
{
	isLoaded_ = false;
	resourceType_ = TYPE::JSON;
}
