#pragma once
#include "Resource.h"
class ImageArrayResource :  public Resource
{
public:
	// コンストラクタ
	ImageArrayResource(void) = delete;
	ImageArrayResource(nlohmann::json json);
	// デストラクタ
	~ImageArrayResource(void)override;

	// ロード
	bool Load(void) override;

	// ロードされているか
	bool IsLoaded(void) const override;

	std::vector<int> GetHandleIds(void) const { return handleIds_; }
private:
	std::vector<int> handleIds_; // 複数画像のハンドルID配列
	int numX_; // 画像の横方向の分割数
	int numY_; // 画像の縦方向の分割数
	int sizeX_; // 分割された1枚あたりの幅
	int sizeY_; // 分割された1枚あたりの高さ

	// 画像の情報をロード
	void LoadResourceInfo(void) override;
	// 画像の完全初期化
	void Init(void) override;

};

