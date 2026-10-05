#pragma once
#include "Resource.h"
class FontResource : public Resource
{
public:

	// コンストラクタ
	FontResource(void) = delete;
	FontResource(nlohmann::json json);
	// デストラクタ
	~FontResource(void)override;

	// ロード
	bool Load(void) override;

	// ハンドルIDを取得する
	const int GetHandleId(void) const { return handleId_; }

	// ロードされているか
	bool IsLoaded(void) const override;

	//ロードエラーがあるか　あればtrue
	bool IsLoadError(void) { return isLoadError_; }

	const int GetSize(void) { return size_; }
private:

	bool isLoadError_;

	//通常の文字サイズ
	static constexpr int DEFAULT_SIZE = 24;

	//通常の文字の太さ
	static constexpr int DEFAULT_THICK = 3;

	static constexpr int FONT_TYPE = DX_FONTTYPE_NORMAL;

	//フォント名
	std::string name_;

	//文字サイズ
	int size_;

	//文字の太さ
	int thick_;

	//ハンドルID
	int handleId_;


	// 情報をロード
	void LoadResourceInfo(void) override;
	// 完全初期化
	void Init(void) override;
};