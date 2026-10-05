#include <fstream>
#include "../../../../Manager/SceneManager.h"
#include "../../../../Utility/Utility.h"
#include "PlayerAnimation.h"

PlayerAnimation::PlayerAnimation(std::string path)
{
	path_ = path;
	LoadAnimation();
	Init();
}

void PlayerAnimation::Init(void)
{
	currentTime_ = 0.0f;
	isEnd_ = false;
}

void PlayerAnimation::Update(void)
{
	//時間を進める
	currentTime_ += SceneManager::GetInstance().GetDeltaTime();
	if (currentTime_ > length_)
	{
		if (isLoop_)
		{
			currentTime_ = std::fmod(currentTime_, length_);
		}
		else
		{
			isEnd_ = true;
			currentTime_ = length_;
		}
	}
}

std::vector<PlayerModel::ModelInfo> PlayerAnimation::ApplyAnimation(const std::vector<PlayerModel::ModelInfo> modelInfos, std::vector<std::string>& appliedMask)
{
	std::vector<PlayerModel::ModelInfo> result = modelInfos;
	//キーフレームを線形補完して現在の時間に適用する
	//現在の時間に一番近い前フレームと次フレームを探す
	float prevTime = 0.0f;
	float nextTime = length_;
	for (auto& keyFrame : keyframes_)
	{
		if (currentTime_ < keyFrame.first)
		{
			nextTime = keyFrame.first;
			break;
		}
		prevTime = keyFrame.first;
	}
	for (auto& modelInfo : result)
	{
		//マスクに含まれているパーツはアニメーションを適用しない
		if (std::find(appliedMask.begin(), appliedMask.end(), modelInfo.part.name) != appliedMask.end())
		{
			continue;
		}
		//前フレームの回転角度を探す
		VECTOR prevRot = Utility::VECTOR_ZERO;
		auto& prevKeyframes = keyframes_[prevTime];
		for (auto& part : prevKeyframes)
		{
			if (part.name == modelInfo.part.name)
			{
				prevRot = part.rotDeg.ToVECTOR();
				break;
			}
		}
		//次フレームの回転角度を探す
		VECTOR nextRot = Utility::VECTOR_ZERO;
		auto& nextKeyframes = keyframes_[nextTime];
		for (auto& part : nextKeyframes)
		{
			if (part.name == modelInfo.part.name)
			{
				nextRot = part.rotDeg.ToVECTOR();
				break;
			}
		}
		//回転角度が両方とも0の場合はアニメーションを適用しない
		if (Utility::EqualsVZero(prevRot) && Utility::EqualsVZero(nextRot))
		{
			continue;
		}
		//現在の時間に対する前フレームと次フレームの重みを計算する
		float t = (currentTime_ - prevTime) / (nextTime - prevTime);
		VECTOR rot = Utility::Lerp(prevRot, nextRot, t);
		auto& polygonInfos = modelInfo.polygonInfo.vertex;
		for (auto& polygonInfo : polygonInfos)
		{
			VECTOR pos = Utility::ApplyRotation(polygonInfo.pos, modelInfo.part.pivot.ToVECTOR(), rot);
			polygonInfo.pos = pos;
			polygonInfo.norm = VNorm(Utility::ApplyRotation(polygonInfo.norm, modelInfo.part.pivot.ToVECTOR(), rot));
		}
	}
	return result;
}

void PlayerAnimation::LoadAnimation(void)
{
	std::ifstream file(path_);
	file >> animationJson_;
	name_ = animationJson_["Name"];
	priority_ = animationJson_["Priority"];
	length_ = animationJson_["Length"];
	layer_ = animationJson_["Layer"];
	mask_ = animationJson_["Mask"].get<std::vector<std::string>>();
	isLoop_ = animationJson_["Loop"];
	auto& keyframes = animationJson_["KeyFrames"];
	for (auto& keyframe : keyframes)
	{
		float time = keyframe["Time"];
		std::vector<Bone> bones;
		for (auto& bone : keyframe["Parts"].items())
		{
			Bone boneInfo;
			boneInfo.name = bone.key();
			auto& rot = bone.value()["Rotation"];
			boneInfo.rotDeg = IntVector3(rot["x"], rot["y"], rot["z"]);
			bones.push_back(boneInfo);
		}
		keyframes_[time] = bones;
	}
}
