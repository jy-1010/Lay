#pragma once
#include "DragonActionUpdateBase.h"
#include "../../../../../../Common/Vector.h"

class DragonActionUpdateLanding :  public DragonActionUpdateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dragonTrans">ドラゴンのトランスフォーム</param>
	/// <param name="json">行動のjson</param>
	DragonActionUpdateLanding(std::weak_ptr<Transform>dragonTrans, nlohmann::json json, DragonAction& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonActionUpdateLanding(void)override;

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

	IntVector3 destinationMapPos_;

	//目的地
	VECTOR destinationPos_;

	VECTOR moveDir_;

	// jsonのデータをロードする
	void Load(void)override;
	//次の移動方向を決める
	void CalcNextDir(void);
	//移動処理
	void Move(void);

};

