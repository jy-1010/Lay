#include "ImageArrayResource.h"

ImageArrayResource::ImageArrayResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
	if(IsPreLoad())
	{
		SetUseASyncLoadFlag(true);
		Load();
		SetUseASyncLoadFlag(false);
	}
}

ImageArrayResource::~ImageArrayResource(void)
{
	for (int handleId : handleIds_)
	{
		DeleteGraph(handleId);
	}
}

bool ImageArrayResource::Load(void)
{
	LoadDivGraph(path_.c_str(), numX_ * numY_, numX_, numY_, sizeX_, sizeY_, handleIds_.data());
	return !handleIds_.empty() && handleIds_[0] != -1;
}

bool ImageArrayResource::IsLoaded(void) const
{
	return !handleIds_.empty() && handleIds_[0] != -1;
}

void ImageArrayResource::LoadResourceInfo(void)
{
	Init();
	numX_ = json_["NumX"].get<int>();
	numY_ = json_["NumY"].get<int>();
	sizeX_ = json_["SizeX"].get<int>();
	sizeY_ = json_["SizeY"].get<int>();
}

void ImageArrayResource::Init(void)
{
	resourceType_ = TYPE::IMAGEARRAY;
	handleIds_.clear();
	numX_ = 0;
	numY_ = 0;
	sizeX_ = 0;
	sizeY_ = 0;
}
