#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../../../../Lib/nlohmann/json.hpp"

class Skin;

class SkinManager
{
public:
	static void CreateInstance(void);
	static SkinManager& GetInstance(void);

	//初期化
	void Init(void);
	//解放
	void Release(void);
	//インスタンス破壊
	void Destroy(void);

	//画像のハンドルIDを取得する
	const int GetHandleId(const std::string key);

	//画像の名前をすべて取得する
	std::vector<std::string> GetSkinNameAll(void);

	//今登録されているスキンの数を取得する
	const int GetSkinNum(void);

	//スキンをマネージャーに保存セーブする
	void SaveSkin(const std::string name);

	//スキンのパスを取得
	std::string GetRootPath(void) { return rootPath_; }

	//選択されたスキンの名前を取得する
	std::string GetSelectedSkinName(void);

	std::string GetDefaultSkinName(void) const { return defaultSkinName_; }

	void SetSelectedSkinName(const std::string skinName) { selectedSkinName_ = skinName; }

	//指定された名前のスキンが存在するか
	bool IsContainName(const std::string skinName) { return skinResources_.contains(skinName); }

	//スキンの画像が存在するかを確認しなければリソース削除
	void CheckSkinResource(void);

private:

	//選択されたスキンの名前
	std::string selectedSkinName_;

	//デフォルトスキン
	std::string defaultSkinName_;

	static SkinManager* instance_;
	
	std::map<std::string,std::shared_ptr<Skin>> skinResources_;
	nlohmann::json skinJson_;

	std::string rootPath_;

	void AddSkin(const std::string name, const std::string path, bool isPreload);
	//新規スキンをjsonに保存
	void AddSkinJson(const std::string name,const std::string path);


	//スキンをマネージャーから削除する
	void DeleteSkin(const std::string name);
	//jsonから削除する
	void DeleteSkinJson(const std::string name);

	SkinManager() = default;
	~SkinManager() = default;
};

