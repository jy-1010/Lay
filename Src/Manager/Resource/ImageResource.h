#pragma once
#include "Resource.h"
class ImageResource :  public Resource
{
public:
    // コンストラクタ
    ImageResource(void) = delete;
    ImageResource(nlohmann::json json);
	// デストラクタ
	~ImageResource(void)override;

    // ロード
	bool Load(void) override;

	// ロードされているか
	bool IsLoaded(void) const override;

	// ハンドルIDを取得する
	const int GetHandleId(void) const { return handleId_; }
private:

	int handleId_; // 画像のハンドルID

    // 画像の情報をロード
	void LoadResourceInfo(void) override;
	// 画像の完全初期化
	void Init(void) override;
};

