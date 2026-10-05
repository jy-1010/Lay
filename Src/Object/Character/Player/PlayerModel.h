#pragma once
#include "../CharacterModelBase.h"
#include "../../Renderer/Polygon3DRenderer.h"

class PlayerAnimationManager;

class PlayerModel : public CharacterModelBase
{
public:

	//親の情報を引き継ぐか
	struct AffectParent
	{
		bool isRot = false;
		bool isPos = false;
		const bool IsAffect(void)const { return isRot || isPos; }
	};

	//キューブ情報
	struct Cube
	{
		IntVector3 offset;	//キューブの中心からのオフセット
		IntVector3 pixelSize;
		FloatVector2 uvOffset;
	};

	//モデルのパーツごとの情報
	struct Model_Part
	{
		std::string name;
		std::string parentName;
		AffectParent affectParent;
		IntVector3 pivot;	//回転中心
		std::vector<Cube> cubes;
	};

	struct ModelInfo
	{
		Model_Part part;
		Polygon3DRenderer::PolygonInfo polygonInfo;
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="skinName">今回使用するスキンの名前</param>
	/// <param name="params">プレイヤーの情報json</param>
	PlayerModel(std::string skinName, nlohmann::json& params);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~PlayerModel(void)override;

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
	/// スキンのハンドルを適用
	/// </summary>
	/// <param name="handle">ハンドルID</param>
	void SetSkinHandle(int handle);

	/// <summary>
	/// アニメーションを割り当てる
	/// </summary>
	/// <param name="animName">アニメーションの名前</param>
	/// <param name="isCompulsion">強制的に変えるか</param>
	void SetAnimation(std::string animName, bool isCompulsion);

	/// <summary>
	/// 頂点座標を適用する
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">大きさ</param>
	void ApplayVertexPos(VECTOR pos, VECTOR rot,float scale);
	
	/// <summary>
	/// アイドルアニメーションを再生中かどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsAnimIdle(void)const;

	/// <summary>
	/// 指定のアニメーションが再生中かどうか
	/// </summary>
	/// <param name="name">アニメーションの名前</param>
	/// <returns></returns>
	bool IsPlayAnim(std::string name)const;

private:
	//アニメーションマネージャー
	std::unique_ptr<PlayerAnimationManager> animationManager_;

	//ピクセル数
	int pixelNum_;

	//スキンのハンドル
	int skinHandle_;

	//テクスチャのサイズ
	FloatVector2 textureSize_;

	//モデルの大親の名前
	std::string rootPartName_;

	//モデルのパーツの情報
	std::vector <Model_Part> modelParts_;

	//描画に必要な情報
	Polygon3DRenderer::PolygonInfo polygonInfo_;

	//パーツの名前とモデルの情報のマップ
	std::vector<ModelInfo> modelInfos_;

	//ピクセルマテリアル
	std::unique_ptr<Polygon3DMaterial> material_;
	//ポリゴンレンダラー
	std::shared_ptr<Polygon3DRenderer> renderer_;

	//モデルの情報を読み込む
	void LoadModelInfo(void)override;

	//スキンを読み込む
	void LoadSkin(std::string skinName);

	//ポリゴンの情報を作成する
	void MakePokygonInfo(void);

	//シェーダの情報を設定する
	void SetRendererInfo(void)override;

	//座標を適用
	void ApplyPosition(VECTOR pos);

	//回転を適用
	void ApplyRotation(VECTOR rot);

	//大きさを適用
	void ApplyScale(float scale);
};

