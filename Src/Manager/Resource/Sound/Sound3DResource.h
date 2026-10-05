#pragma once
#include "SoundResource.h"
class Sound3DResource :  public SoundResource
{
public:
	// コンストラクタ
	Sound3DResource(void) = delete;
	Sound3DResource(nlohmann::json json);
	// デストラクタ
	~Sound3DResource(void)override;

	// ロード
	bool Load(void) override;

	void Play(void)override;
private:

	float radius_; // 音が聞こえる範囲
	VECTOR pos_; // 音の位置
	// 3D音響の情報をロード
	void LoadResourceInfo(void) override;
	// 3D音響の完全初期化
	void Init(void) override;
};

