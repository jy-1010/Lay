#include "ModelResource.h"

ModelResource::ModelResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
	if(IsPreLoad())
	{
		SetUseASyncLoadFlag(true);
		Load();
		SetUseASyncLoadFlag(false);
	}
}

ModelResource::~ModelResource(void)
{
	MV1DeleteModel(handleId_);
}

bool ModelResource::Load(void)
{
	handleId_ = MV1LoadModel(path_.c_str());
	return handleId_ != -1;
}

bool ModelResource::IsLoaded(void) const
{
	return handleId_ != -1;
}

const int ModelResource::GetHandleId(void)
{
	int i = MV1DuplicateModel(handleId_);
	handleIds_.push_back(i);
	return i;
}

void ModelResource::LoadResourceInfo(void)
{
	Init();
}

void ModelResource::Init(void)
{
	resourceType_ = TYPE::MODEL;
	handleId_ = -1;
}
