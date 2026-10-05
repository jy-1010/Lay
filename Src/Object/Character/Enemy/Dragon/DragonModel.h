#pragma once
#include <memory>
#include "../../CharacterModelBase.h"
#include "../../../../Renderer/ModelRenderer.h"

class Transform;
class AnimationController;

class DragonModel : public CharacterModelBase
{
public:

	//通常時の色の倍率
	static constexpr FLOAT4 DEFAULT_COLOR_RATE = { 1.0f,1.0f,1.0f,1.0f };

	//ダメージ時の色の倍率
	static constexpr FLOAT4 DAMAGE_COLOR_RATE = { 5.0f,0.5f,0.5f,1.0f };

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="transform">ドラゴンのトランスフォーム</param>
	DragonModel(std::shared_ptr<Transform> transform,nlohmann::json json);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonModel(void)override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void)override;

	/// <summary>
	/// UIの描画処理
	/// </summary>
	/// <param name=""></param>
	void UIDraw(void)override;

	/// <summary>
	/// 色倍率を設定する
	/// </summary>
	/// <param name=""></param>
	void SetColorRate(FLOAT4 colorRate);
private:

	struct AnimationInfo
	{
		std::string animName;	//アニメーションの名前
		float speed;			//アニメーションの速度
		int id;					//アニメーションのID
	};

	// ドラゴンのトランスフォーム
	std::weak_ptr<Transform> transform_;

	std::vector<AnimationInfo> animationInfos_;

	// アニメーションコントローラー
	std::unique_ptr<AnimationController> animationController_;

	//ピクセルマテリアル
	std::unique_ptr<ModelMaterial> material_;
	//ポリゴンレンダラー
	std::shared_ptr<ModelRenderer> renderer_;

	//シェーダの情報を設定する
	void SetRendererInfo(void)override;

	//モデルの情報を読み込む
	void LoadModelInfo(void)override;

	//アニメーションの初期化
	void InitAnimation(void);

	// アニメーション名からアニメーションIDを取得
	int GetAnimationIdByName(const std::string& name) const;
};

