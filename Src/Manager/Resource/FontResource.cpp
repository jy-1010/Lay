#include "FontResource.h"

FontResource::FontResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
	if (IsPreLoad())
	{
		//SetUseASyncLoadFlag(true);
		Load();
		//SetUseASyncLoadFlag(false);
	}
}

FontResource::~FontResource(void)
{
}

bool FontResource::Load(void)
{
	//フォントをゲーム内限定登録
	int result = AddFontResourceExA(path_.c_str(), FR_PRIVATE, nullptr);
	//登録失敗
	if (result == 0)
	{
		isLoadError_ = true;
		return false;
	}

	//ハンドル作成
	handleId_ = CreateFontToHandle(name_.c_str(), size_, thick_, FONT_TYPE);

	//ハンドル作成失敗
	if (handleId_ == -1)
	{
		isLoadError_ = true;
		return false;
	}

	return true;
}

bool FontResource::IsLoaded(void) const
{
	return handleId_ != -1;
}

void FontResource::LoadResourceInfo(void)
{
	Init();
	name_ = json_["Name"];
	size_ = json_["Size"];
	thick_ = json_["Thick"];
}

void FontResource::Init(void)
{
	name_ = "";
	size_ = DEFAULT_SIZE;
	thick_ = DEFAULT_THICK;
	handleId_ = -1;
	isLoadError_ = false;
	resourceType_ = TYPE::FONT;
}
