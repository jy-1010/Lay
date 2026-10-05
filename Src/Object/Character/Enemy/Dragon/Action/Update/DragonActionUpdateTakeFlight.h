#pragma once
#include "DragonActionUpdateBase.h"
class DragonActionUpdateTakeFlight :  public DragonActionUpdateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dragonTrans">ドラゴンのトランスフォーム</param>
	/// <param name="json">行動のjson</param>
	DragonActionUpdateTakeFlight(std::weak_ptr<Transform>dragonTrans, nlohmann::json json, DragonAction& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonActionUpdateTakeFlight(void)override;

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

private:

	//移動速度
	float moveSpeed_;

	//上昇速度
	float riseSpeed_;

	//飛ぶ高さ
	int height_;

	//移動範囲
	int moveRadius_;

	//方向転換時の範囲
	int minChangeDir_;
	int maxChangeDir_;

	float reChangeDelay_;

	float time_;

	// jsonのデータをロードする
	void Load(void)override;
	//移動範囲外に出たか
	bool IsMoveLimit(void);
	//次の移動方向を決める
	void CalcNextDir(void);
	//移動処理
	void Move(void);

};

