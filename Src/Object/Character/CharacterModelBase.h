#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../../Lib/nlohmann/json.hpp"
#include "../../Common/Vector.h"

class CharacterModelBase
{
public:

	//シェーダー情報
	struct ShaderInfo
	{
		std::string VSKey;	// 頂点シェーダーのリソースキー
		std::string PSKey;	// ピクセルシェーダーのリソースキー
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="params">情報</param>
	CharacterModelBase(nlohmann::json& params);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~CharacterModelBase(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void) = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void) = 0;

	/// <summary>
	/// UIの描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void UIDraw(void) = 0;

protected:

	//アニメーションのkey
	std::string animationKey_;

	ShaderInfo shaderInfo_;	// シェーダー情報

	//親のjson
	nlohmann::json& params_;

	//モデルの情報
	nlohmann::json modelInfo_;

	//シェーダの情報を設定する
	virtual void SetRendererInfo(void)=0;

	//モデルの情報を読み込む
	virtual void LoadModelInfo(void) = 0;
private:

};

