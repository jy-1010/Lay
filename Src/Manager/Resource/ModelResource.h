#pragma once
#include "Resource.h"
class ModelResource :  public Resource
{
public:
	// コンストラクタ
	ModelResource(void) = delete;
	ModelResource(nlohmann::json json);
	// デストラクタ
	~ModelResource(void)override;
	// ロード
	bool Load(void) override;
	// ロードされているか
	bool IsLoaded(void) const override;
	// ハンドルIDを取得する
	const int GetHandleId(void);

private:

	int handleId_; // モデルのハンドルID

	std::vector<int> handleIds_;

	// 3Dモデルの情報をロード
	void LoadResourceInfo(void) override;
	// 3Dモデルの完全初期化
	void Init(void) override;
};

