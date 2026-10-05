#pragma once
#include<string>
#include<map>
#include<memory>

#include "Resource/Resource.h"

class ResourceManager
{	
public:
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// ロードする
	void Load(std::string key);

	// 種類ごとのリソースの取得
	std::weak_ptr<JsonResource> GetJsonResource(const std::string& key);
	std::weak_ptr<ImageResource> GetImageResource(const std::string& key);
	std::weak_ptr<ImageArrayResource> GetImageArrayResource(const std::string& key);
	std::weak_ptr<ModelResource> GetModelResource(const std::string& key);
	std::weak_ptr<Sound2DResource> GetSound2DResource(const std::string& key);
	std::weak_ptr<Sound3DResource> GetSound3DResource(const std::string& key);
	std::weak_ptr<ShaderResource> GetShaderResource(const std::string& key);
	std::weak_ptr<FontResource> GetFontResource(const std::string& key);

private:
	// 静的インスタンス
	static ResourceManager* instance_;
	std::map<std::string, std::shared_ptr<Resource>> resourcesMap_;	// 全てのリソース

	Resource::TYPE GetResourceTypeFromString(const std::string&	str);
	std::shared_ptr<Resource> CreateResource( const nlohmann::json& json, Resource::TYPE type);
	std::shared_ptr<Resource> GetResource(const std::string& key);
};

