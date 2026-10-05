#include <DxLib.h>
#include "Skin.h"

Skin::Skin(std::string name, std::string path, bool isPreload)
{
	name_ = name;
	path_ = path;
	if (isPreload)
	{
		SetUseASyncLoadFlag(true);
		Load();
		SetUseASyncLoadFlag(false);
	}
}

Skin::~Skin(void)
{
	if (handle_ != -1)
	{
		DeleteGraph(handle_);
	}
	handle_ = -1;
}

const bool Skin::isLoaded(void) const
{
	bool ret = false;
	int async = CheckHandleASyncLoad(handle_);
	if (handle_ != -1 && !CheckHandleASyncLoad(handle_))
	{
		ret = true;
	}
	return ret;
}

const bool Skin::isLoadError(void) const
{
	bool ret = false;
	int async = CheckHandleASyncLoad(handle_);
	if (handle_ == -1 || async == -1)
	{
		ret = true;
	}
	return ret;
}

void Skin::Load(void)
{
	if (handle_ != -1)
	{
		return;
	}
	handle_ = LoadGraph(path_.c_str());
}

const int Skin::GetHandle(void) const
{
	if (isLoaded())
	{
		return handle_;
	}
	return -1;
}
