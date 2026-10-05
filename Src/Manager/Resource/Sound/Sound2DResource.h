#pragma once
#include "SoundResource.h"
class Sound2DResource : public SoundResource
{
public:

	// コンストラクタ
	Sound2DResource(void) = delete;
	Sound2DResource(nlohmann::json json);
	// デストラクタ
	~Sound2DResource(void)override;
	// ロード
	bool Load(void) override;

	void Play(void)override;
private:
	// 2D音響の情報をロード
	void LoadResourceInfo(void) override;
	// 2D音響の完全初期化
	void Init(void) override;
};

