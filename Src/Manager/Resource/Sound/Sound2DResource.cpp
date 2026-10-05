#include "Sound2DResource.h"

Sound2DResource::Sound2DResource(nlohmann::json json) : SoundResource(json)
{
    LoadResourceInfo();
    if (IsPreLoad())
    {
        //SetUseASyncLoadFlag(true);
        Load();
        //SetUseASyncLoadFlag(false);
	}
}

Sound2DResource::~Sound2DResource(void)
{
}

bool Sound2DResource::Load(void)
{
    handleId_ = LoadSoundMem(path_.c_str());
    return true;
}

void Sound2DResource::Play(void)
{
    int playType = isLoop_ ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
    if (!IsLoaded())
    {
        Load();
        return;
    }
    if (maxPlayNum_ == playHandle_.size())
    {
        for (auto& handle : playHandle_)
        {
            if (CheckSoundMem(handle))
            {
                continue;
            }
            PlaySoundMem(handle, playType);
            return;
        }
    }
    else
    {
        int handle = DuplicateSoundMem(handleId_);
        PlaySoundMem(handle, playType);
        ChangeVolumeSoundMem(maxVolume_, handle);
        playHandle_.push_back(handle);
    }
}

void Sound2DResource::LoadResourceInfo(void)
{
    Init();
}

void Sound2DResource::Init(void)
{
	resourceType_ = TYPE::SOUND_2D;
}
