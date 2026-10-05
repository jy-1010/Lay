#include "SoundResource.h"

SoundResource::SoundResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
}

SoundResource::~SoundResource(void)
{
}

bool SoundResource::IsLoaded(void) const
{
	return handleId_ != -1;
}

void SoundResource::Stop(void)
{
	for (auto& handle : playHandle_)
	{
		StopSoundMem(handle);
	}
}

void SoundResource::LoadResourceInfo(void)
{
	Init();
	std::string soundTypeStr = json_["SoundType"].get<std::string>();
	if (soundTypeStr == "BGM")
	{
		soundType_ = SOUND_TYPE::BGM;
	}
	else if(soundTypeStr == "SE")
	{
		soundType_ = SOUND_TYPE::SE;
	}
	else
	{
		soundType_ = SOUND_TYPE::NONE;
	}
	isLoop_ = json_["Loop"].get<bool>();
	maxVolume_ =static_cast<int>(MAX_VOLUME * json_["MaxVolume"].get<float>());
	maxPlayNum_ = json_["MaxPlayNum"].get<int>();
	pitch_ = json_["Pitch"].get<float>();
}

void SoundResource::Init(void)
{
	soundType_ = SOUND_TYPE::NONE;
	handleId_ = -1;
	pitch_ = 0.0f;
	maxVolume_ = 0;
	maxPlayNum_ = 0;
	isLoop_ = false;
}
