#include "../../../../Application.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource/JsonResource.h"
#include "Skin.h"
#include "SkinManager.h"

SkinManager* SkinManager::instance_ = nullptr;

void SkinManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SkinManager();
	}
	instance_->Init();
}

SkinManager& SkinManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CreateInstance();
	}
	return *instance_;
}

void SkinManager::Init(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	selectedSkinName_ = "";
	auto jsonResource = resourceManager.GetJsonResource("PlayerSkinJson").lock();
	skinJson_ = jsonResource->GetData();
	rootPath_ = Application::PATH_DATA + skinJson_["RootPath"].get<std::string>();
	bool isPreload = skinJson_["Preload"];
	defaultSkinName_ = skinJson_["Default"];
	//lastSavename_ = "";
	//それぞれのスキンのインスタンス作成
	for (auto& skinInfo : skinJson_["Info"])
	{
		std::string name = skinInfo["Name"].get<std::string>();
		std::string path = rootPath_ + skinInfo["Path"].get<std::string>();
		AddSkin(name, path, isPreload);
	}
	CheckSkinResource();
}

void SkinManager::Release(void)
{
	skinResources_.clear();
}

void SkinManager::Destroy(void)
{
	Release();
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

const int SkinManager::GetHandleId(const std::string key)
{
	if (!skinResources_[key]->isLoaded())
	{
		skinResources_[key]->Load();
	}
	return skinResources_[key]->GetHandle();
}

std::vector<std::string> SkinManager::GetSkinNameAll(void)
{
	std::vector<std::string> ret;
	for (auto& skin : skinResources_)
	{
		ret.push_back(skin.first);
	}
	return ret;
}

const int SkinManager::GetSkinNum(void)
{
	return static_cast<int>(skinResources_.size());
}

void SkinManager::SaveSkin(const std::string name)
{
	//無限ループで同じ名前で登録されているものがすでにあるかを確認しある場合数字を追加する
	int i = 0;
	std::string tempName = name;
	for (;;)
	{
		if (!IsContainName(tempName))
		{
			break;
		}
		tempName = name + "_" + std::to_string(i);
	}
	//保存する
	AddSkin(name, rootPath_ + tempName + ".png", true);
	AddSkinJson(tempName, tempName + ".png");
	selectedSkinName_ = tempName;
}

std::string SkinManager::GetSelectedSkinName(void)
{
	if (selectedSkinName_ == "")
	{
		return defaultSkinName_;
	}
	return selectedSkinName_;
}

void SkinManager::CheckSkinResource(void)
{
	//ロードエラーが起きたスキンをjsonからも削除
	std::vector<std::string> names;
	for (auto& skin : skinResources_)
	{
		if (!skin.second->isLoadError())
		{
			continue;
		}
		names.push_back(skin.first);
	}
	for (auto& name : names)
	{
		DeleteSkin(name);
		DeleteSkinJson(name);
	}
}

void SkinManager::AddSkin(const std::string name, const std::string path, bool isPreload)
{
	skinResources_[name] = std::make_shared<Skin>(name, path, isPreload);
}

void SkinManager::AddSkinJson(const std::string name, const std::string path)
{
	//中身を書き換える
	nlohmann::json json;
	json["Name"] = name;
	json["Path"] = path;
	skinJson_["Info"].push_back(json);
	skinJson_.dump(2);
	//情報を更新する
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource("PlayerSkinJson").lock();
	jsonResource->OutFile(skinJson_);
}

void SkinManager::DeleteSkin(const std::string name)
{
	skinResources_.erase(name);
}

void SkinManager::DeleteSkinJson(const std::string name)
{
	nlohmann::json json;
	for (auto& skin : skinResources_)
	{
		nlohmann::json temp;
		//指定されたモノをスキップする
		if (skin.first == name)
		{
			continue;
		}
		temp["Name"] = skin.first;
		temp["Path"] = skin.second->GetPath();
		json.push_back(temp);
	}
	skinJson_["Info"] = json;
	skinJson_.dump(2);
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource("PlayerSkinJson").lock();
	jsonResource->OutFile(skinJson_);
}
