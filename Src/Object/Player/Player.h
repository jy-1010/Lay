#pragma once
#include"../ObjectBase.h"

class Player : public ObjectBase
{
public:

	//コンストラクタ
	Player(int _index);
	
	//デストラクタ
	~Player(void);

	//読み込み
	void Load(void) override;

	//初期化
	void Init(void) override;

	//更新
	void Update(void) override;

	//描画
	void Draw(void) override;

	//それぞれの当たり判定後の処理
	void OnHit(const Collider* _hitCol) override;

private:

	//基本移動速度
	float MOVE_SPEED = 10.0f;

	//プレイヤー番号
	int index_;

	//移動速度
	float moveSpeed_;

	//移動
	void Move(void);
};