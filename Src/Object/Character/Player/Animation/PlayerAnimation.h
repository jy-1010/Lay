#pragma once
#include <string>
#include <vector>
#include "../../../../Common/Vector.h"
#include "../../../../Lib/nlohmann/json.hpp"
#include "../PlayerModel.h"

class PlayerAnimation
{
public:
	//コンストラクタ アニメーションのjsonファイルのパスを渡す
	PlayerAnimation(std::string path);
	//アニメーションの初期化
	void Init(void);
	//アニメーションの更新
	void Update(void);
	//アニメーションの終了判定
	bool IsEnd(void)const { return isEnd_; }
	//レイヤーの取得
	int GetLayer(void)const { return layer_; }
	//優先度の取得
	int GetPriority(void)const { return priority_; }
	//マスクの取得
	std::vector<std::string> GetMask(void)const { return mask_; }
	//アニメーションの適用
	std::vector<PlayerModel::ModelInfo> ApplyAnimation(const std::vector<PlayerModel::ModelInfo> modelInfos, std::vector<std::string>& appliedMask);

private:

	void LoadAnimation(void);

	struct Bone
	{
		std::string name; // ボーンの名前
		IntVector3 rotDeg; // 回転角度
	};
	//アニメーションが終了したか
	bool isEnd_;
	//jsonファイル
	nlohmann::json animationJson_;
	//アニメーションのパス
	std::string path_;
	//アニメーションの優先度
	int priority_;
	//名前
	std::string name_;
	//総時間
	float length_;
	//現在の時間
	float currentTime_;
	//レイヤー
	int layer_;
	//マスク
	std::vector<std::string> mask_;
	//ループするかどうか
	bool isLoop_;
	//キーフレームの情報
	std::map<float, std::vector<Bone>> keyframes_;
};

