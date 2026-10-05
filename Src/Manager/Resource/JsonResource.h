#pragma once
#include <fstream>
#include "Resource.h"
class JsonResource : public Resource
{
public:
	// コンストラクタ
	JsonResource(void) = delete;
	JsonResource(nlohmann::json json);
	// デストラクタ
	~JsonResource(void)override;

	// ロード
	bool Load(void)override;

	// ロードされているか
	bool IsLoaded(void) const override;

	// JSONの内容を取得する
	const nlohmann::json& GetData(void) const { return data_; }

	//jsonに書き込む
	void OutFile(nlohmann::json data);

protected:
	bool isLoaded_; // ロードされているか

	std::fstream jsonFile_;

	nlohmann::json data_; // JSONの内容
	// JSONの情報をロード
	void LoadResourceInfo(void) override;
	// JSONの完全初期化
	void Init(void) override;

};
