#pragma once
#include <vector>
#include <memory>
#include <string>
#include "../../../../../Renderer/Polygon3DRenderer.h"

class DragonBreath
{
public:

	enum class STATE
	{
		FLIGHT,	//空中状態
		SPRASH,	//着地後の拡散
		POISON_AREA,	//毒エリア
		FINISH,	//終了
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	DragonBreath(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonBreath(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// ブレスの作成
	/// </summary>
	/// <param name="startPos">ドラゴンの座標</param>
	/// <param name="targetPos">プレイヤーの座標</param>
	void CreateBreath(VECTOR startPos,VECTOR targetPos);

private:

	struct Rect
	{
		int left = -1;	//左
		int top = -1;	//上
		int right = -1;	//右
		int bottom = -1;	//下
	};

	struct JsonInfo
	{
		float speed = -1.0f;	//空中時の速度
		float flyTime = -1.0f;	//空中滞在時間
		float duration = -1.0f;	//エリア展開後継続時間
		float radius = -1.0f;	//エリアの半径
		int drawEffectNum = -1;	//エリアのエフェクト描画数
		float effectSpeed = -1.0f;	//エフェクトのスピード

		std::string key = "";	//画像のキー
		int handleId = -1;		//ハンドルID
		IntVector2 textureSize = { -1,-1 };	//画像の大きさ
		Rect AreaTextureSize;	//展開時のエフェクト画像のサイズ
		Rect BallTextureSize;	//空中時のエフェクト画像のサイズ
	};

	struct Info
	{
		STATE state = STATE::FLIGHT;	//状態
		VECTOR startPos;	//スタート座標
		VECTOR pos;		//中心座標
		VECTOR targetPos;	//目標座標
		std::vector<VECTOR> effectPos;	//エフェクト座標
		bool isEnd = false;	//終わったか　
	};

	//描画情報
	std::vector<Info> drawInfo_;

	//ポリゴン情報
	Polygon3DRenderer::PolygonInfo polygonInfo_;

	//ポリゴンマテリアル
	std::unique_ptr<Polygon3DMaterial> material_;
	//ポリゴンレンダラー
	std::shared_ptr<Polygon3DRenderer> renderer_;

	void LoadData(void);

};

