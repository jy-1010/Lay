#pragma once
#include<string>
#include<DxLib.h>
#include "../Lib/nlohmann/json.hpp"

class JsonResource;
class ImageResource;
class ImageArrayResource;
class ModelResource;
class Sound2DResource;
class Sound3DResource;
class ShaderResource;
class FontResource;

class Resource
{
public:

	// リソースタイプ
	enum class TYPE
	{
		NONE,	
		JSON,		// JSONファイル
		IMAGE,		// 画像
		IMAGEARRAY, // 複数画像
		MODEL,		// 3Dモデル
		SOUND_2D,	// 2D音響
		SOUND_3D,	// 3D音響
		SHADER,		// シェーダー
		FONT,		// フォント
	};



	// コンストラクタ
	Resource(void) = delete;
	Resource(nlohmann::json json);

	//デストラクタ
	virtual ~Resource(void) = default;

	//ロード
	virtual bool Load(void) = 0;

	//ロードされているか
	virtual bool IsLoaded(void) const = 0;

	//リソースタイプを取得する
	TYPE GetResourceType(void) const { return resourceType_; }

	//ファイルのパスを取得する
	const std::string& GetPath(void) const { return path_; }

	//json情報を取得する
	const nlohmann::json& GetJson(void) const { return json_; }

	//リソースをキャストする
	static std::shared_ptr<JsonResource> CastToJsonResource(std::shared_ptr<Resource> resource);
	static std::shared_ptr<ImageResource> CastToImageResource(std::shared_ptr<Resource> resource);
	static std::shared_ptr<ImageArrayResource> CastToImageArrayResource(std::shared_ptr<Resource> resource);
	static std::shared_ptr<ModelResource> CastToModelResource(std::shared_ptr<Resource> resource);
	static std::shared_ptr<Sound2DResource> CastToSound2DResource(std::shared_ptr<Resource> resource);
	static std::shared_ptr<Sound3DResource> CastToSound3DResource(std::shared_ptr<Resource> resource);
	static std::shared_ptr<ShaderResource> CastToShaderResource(std::shared_ptr<Resource> resource);
	static std::shared_ptr<FontResource> CastToFontResource(std::shared_ptr<Resource> resource);
protected:

	//タイプ
	TYPE resourceType_;

	//json情報
	nlohmann::json json_;

	//ファイルのパス
	std::string path_;

	//リソースの情報をロード
	virtual void LoadResourceInfo(void);
	//プリロードを行うか
	bool IsPreLoad(void)const;
	//リソースの完全初期化
	virtual void Init(void) = 0;
};