#include "Sound3DResource.h"

Sound3DResource::Sound3DResource(nlohmann::json json) : SoundResource(json)
{
    LoadResourceInfo();
    if (IsPreLoad())
    {
        SetUseASyncLoadFlag(true);
        Load();
        SetUseASyncLoadFlag(false);
    }
}

Sound3DResource::~Sound3DResource(void)
{
}

bool Sound3DResource::Load(void)
{
    return false;
}

void Sound3DResource::Play(void)
{
}

void Sound3DResource::LoadResourceInfo(void)
{
	Init();
}

void Sound3DResource::Init(void)
{
	resourceType_ = TYPE::SOUND_3D;
}
