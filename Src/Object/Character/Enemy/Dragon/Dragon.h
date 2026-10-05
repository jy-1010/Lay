#pragma once
#include <map>
#include "../../../ObjectBase.h"

class DragonModel;
class DragonActionManager;
class Player;

class Dragon :  public ObjectBase
{
public:

	struct Param
	{
		float DEFAULT_SCALE;	//通常サイズ
		float MAX_HEALTH;	//最大体力
		float health;		//体力
		float DAMAGE_DERAY;
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Dragon(const Player& player, float blockSize = -1.0f);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Dragon(void)override;

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
	/// 座標を設定する
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(VECTOR pos) { transform_->pos = pos; }

	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;


	bool IsDead(void) { return isDead_; }
private:
	
	bool isDead_;

	float damageDelayTime_;

	//１ブロックの大きさ
	float blockSize_;

	std::unique_ptr<DragonActionManager> actionManager_;

	// プレイヤー
	const Player& player_;

	//ドラゴンの情報json 
	nlohmann::json paramsJson_;

	std::unique_ptr<DragonModel> model_;

	Param params_;

	std::map<int, VECTOR> bornPos_;

	void LoadDragonInfo(void);

	void InitCollider(void);

	void UpdateFramePos(void);

	void Damage(float damage);

	static constexpr int BAR_WIDTH = 364;
	static constexpr int BAR_HEIGHT = 30;
	static constexpr int MARGIN = 10;
	void DrawHPUI(void);
};

