#include "ImageResource.h"

ImageResource::ImageResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
	if(IsPreLoad())
	{
		SetUseASyncLoadFlag(true);
		Load();
		SetUseASyncLoadFlag(false);
	}
}

ImageResource::~ImageResource(void)
{
	if (handleId_ != -1)
	{
		DeleteGraph(handleId_);
	}
}

bool ImageResource::Load(void)
{
	handleId_ = LoadGraph(path_.c_str());
	return handleId_ != -1;
}

bool ImageResource::IsLoaded(void) const
{
	return handleId_ != -1;
}

void ImageResource::LoadResourceInfo(void)
{
	Init();
}

void ImageResource::Init(void)
{
	resourceType_ = TYPE::IMAGE;
	handleId_ = -1;
}
