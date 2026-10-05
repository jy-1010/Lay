#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "../../../../Lib/nlohmann/json.hpp"
#include "../PlayerModel.h"

class PlayerAnimation;

class PlayerAnimationManager
{
public:
	//コンストラクタ モデルの情報を渡す
	PlayerAnimationManager(const std::vector<PlayerModel::ModelInfo>& modelInfos, std::string key, std::string rootPartName);

	//アニメーションの初期化
	void Init(void);
	void Init(std::string name);

	//アニメーションの更新
	void Update(void);

	//アニメーションのsetセットするアニメーションの名前とアニメーションをクリアするかどうか
	void SetAnimation(std::string name);

	//アニメションの停止
	void StopAnimation(std::string name);

	//現在のアニメーションの名前を取得する
	const std::vector<std::string> GetCurrentAnimationNames(void)const { return animationNames; }

	//ポリゴン情報を取得する
	Polygon3DRenderer::PolygonInfo GetPolygonInfo(void)const;

	//アニメーションを適用する
	void ApplyAnimation(void);

private:
	//アニメーションの情報
	nlohmann::json animationInfo_;

	//モデルの大親の名前
	std::string rootPartName_;

	//デフォルトのアニメーションの名前
	std::string defaultAnimationName_;

	//モデルの情報
	const std::vector<PlayerModel::ModelInfo>& modelInfos_;

	//ポリゴン情報
	Polygon3DRenderer::PolygonInfo polygonInfo_;

	//アニメーションの名前とアニメーションのマップ
	std::map<std::string, std::shared_ptr<PlayerAnimation>> animations;

	//現在のアニメーションの名前
	std::vector<std::string> animationNames;

	//アニメーションの追加
	void AddAnimation(std::string name, std::shared_ptr<PlayerAnimation> animation);

};

