#pragma once
#include <map>
#include <vector>
#include <memory>

class InstanceData;

class DrawInstancing
{
public:

    enum class TYPE
    {
		WOOD_L, // 大きい木
		WOOD_M,// 中くらいの木
		WOOD_S,// 小さい木
		WOOD_PLATE,// 木の板
		STONE_L,// 大きい石
		STONE_M,// 中くらいの石
		STONE_S,// 小さい石
		STONE_PLATE,// 石の板
		ROCK,// 岩
		RAIL_STRAIGHT,// 直線のレール
		RAIL_CURVE,// 曲線のレール
		WATER,// 水
		BRIDGE,// 橋
		PLANE,// 平面
    };

	// インスタンス生成
	static void CreateInstance(void);

	// インスタンス取得
	static DrawInstancing& GetInstance(void);

	// 初期化
	void Init(void);

	//　描画
	void Draw(void);

	// インスタンス破棄（メモリ解放）
	void Destroy(void);

	// インスタンスデータの追加
	void AddInstanceData(TYPE type, std::shared_ptr<InstanceData> instanceData);

	// インスタンスデータの種類を変更
	bool ChangeType(TYPE oldType, TYPE newType,const std::shared_ptr<InstanceData>& instanceData);

private:

	// 静的インスタンス
	static DrawInstancing* instance_;

	//種類ごとにインスタンスデータを格納するマップ
	std::map<TYPE,std::vector<std::shared_ptr<InstanceData>>> instanceData_;

	// コンストラクタ
	DrawInstancing(void);
	// コピーの禁止
	DrawInstancing(const DrawInstancing& _copy) = delete;
	// コピー代入の禁止
	DrawInstancing& operator= (const DrawInstancing& _copy) = delete;
	// デストラクタ
	~DrawInstancing(void);
};