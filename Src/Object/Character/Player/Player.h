#pragma once
#include "../../ObjectBase.h"

class PlayerModel;

class Player :  public ObjectBase
{
public:

	struct Param
	{
		FLOAT3 COLLISION_SIZE;	//当たり判定の大きさ
		float HEAD_HEIGHT;	//頭の高さ
		float MAX_HEALTH;	//最大体力
		float BLOCK_REACH;	//ブロックに対するリーチ
		float ENTITY_REACH;	//エンティティに対するリーチ
		float WALK_SPEED;	//歩くスピード
		float SPRINT_SPEED;	//ダッシュスピード
		float JUMP_POWER;	//ジャンプスピード
		float SWORD_DAMAGE;	//剣のダメージ
		float ARROW_DAMAGE;	//弓のダメージ
		float DAMAGE_DERAY;
		float health;		//体力
		float DefaultScale;	//通常サイズ
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="skinName">スキンの名前</param>
	Player(std::string skinName,float blockSize = -1.0f);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Player(void)override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void) override;

	/// <summary>
	/// UI描画
	/// </summary>
	/// <param name=""></param>
	void UIDraw(void) override;

	/// <summary>
	/// スキンのハンドルIDを反映する
	/// </summary>
	/// <param name="handle">画像ハンドルID</param>
	void SetSkinHandle(int handle);

	/// <summary>
	/// アニメーションを割り当てる
	/// </summary>
	/// <param name="animName">アニメーションの名前</param>
	/// <param name="isCompulsion">強制的に変えるか</param>
	void SetAnimation(std::string animName,bool isCompulsion);

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const VECTOR pos) { transform_->pos = pos; }

	/// <summary>
	/// 頂点情報を適用する
	/// </summary>
	/// <param name=""></param>
	void ApplyVertex(void);

	std::weak_ptr<Transform> GetHeadTransform(void) { return headTrans_; }

	/// <summary>
	/// ダメージを取得
	/// </summary>
	/// <param name="tag">どの武器のタグか</param>
	/// <returns></returns>
	const float GetDamage(Collider::TAG tag)const;

	/// <summary>
	/// 衝突時の反応
	/// </summary>
	/// <param name="_hitCol">当たった相手コライダー</param>
	/// <param name="hitPos">当たった場所</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;

	/// <summary>
	/// 体力を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetHP(void)const { return params_.health; }

	bool IsDead(void) { return isDead_; }
private:

	bool isDead_;

	float damageDeray_;

	//頭座標のトランスフォーム
	std::shared_ptr<Transform> headTrans_;

	VECTOR lineEndPos_;

	//モデル管理
	std::unique_ptr<PlayerModel> model_;
	//プレイヤーの情報json 
	nlohmann::json paramsJson_;
	//パラメーター
	Param params_;

	float blockSize_;

	//プレイヤー情報の読み込み
	void LoadPlayerInfo(void);

	//攻撃処理
	void UpdateAttack(void);

	//移動処理
	void UpdateMove(void);

	//移動方向をもとに回転量を取得
	VECTOR CalcRot(VECTOR dir);
	VECTOR CalcRot(IntVector2 dir);

	//頭座標を計算
	void CalcHeadPos(void);

	void InitCollider(void);

	static constexpr int BAR_WIDTH = 100;
	static constexpr int BAR_HEIGHT = 30;
	static constexpr int MARGIN = 10;
	void DrawHPUI(void);
};

