#pragma once
#include "../Resource.h"
class SoundResource :  public Resource
{
public:
	//３分間のミリ秒
	static constexpr int THREE_MINIT_MILI_TIME = 180000;
	//音量の最大値
	static constexpr int MAX_VOLUME = 255;
	enum class SOUND_TYPE
	{
		NONE,
		BGM, // BGM
		SE,  // SE
	};

	// コンストラクタ
	SoundResource(void) = delete;
	SoundResource(nlohmann::json json);
	// デストラクタ
	virtual ~SoundResource(void)override;

	// ロード
	virtual bool Load(void) override = 0;
	// ロードされているか
	bool IsLoaded(void) const override;

	//ハンドルIDを取得する
	const int GetHandleId(void) const { return handleId_; }

	virtual void Play(void) = 0;

	void Stop(void);

protected:
	// 音の種類
	SOUND_TYPE soundType_;
	// 音のハンドルID
	int handleId_;
	// ピッチの高さ
	float pitch_;
	// 音量の最大値
	int maxVolume_;
	// 同時再生数の最大値
	int maxPlayNum_;
	// ループ再生するかどうか
	bool isLoop_;
	
	std::vector<int> playHandle_;

	// 音の情報をロード
	virtual void LoadResourceInfo(void) override;
	// 音の完全初期化
	virtual void Init(void) override;
};

