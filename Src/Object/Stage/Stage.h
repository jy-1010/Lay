#pragma once
#include <map>
#include <thread>
#include "../ObjectBase.h"
#include "../../Common/vector.h"
#include "../../Renderer/Polygon3DRenderer.h"

class BlockInfo;
class Chunk;

class Stage : public ObjectBase
{
public:

	//jsonから取得する情報
	struct JsonInput
	{
		int radius = -1;	//島の半径
		int baseHeight = -1;	//島の基準硬度
		int outHeight = -1;	//島の外側の高さ
		std::vector<int> pillarHeights;	//黒曜石の柱の高さ
		int pillarNum = -1;	//柱の数
		int pillarHasCageNum = -1;	//柱に鉄格子がついている数
		int chanckBlockNum = -1;	//1チャンクのブロックのサイズ
		int chankMaxVertexNum = -1;	//1チャンクの最大頂点数
		int chankMaxIndexNum = -1;	//1チャンクの頂点配列の最大数
		IntVector3 playerSpawnMapPos = {};
		IntVector3 enemySpawnMapPos = {};
	};

	Stage(void);
	~Stage(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UIDraw(void) override;
	VECTOR GetPlayerSpawnPos(void);
	VECTOR GetEnemySpawnPos(void);

	const JsonInput& GetJsonInput(void)const { return jsonInput_; }

	//変換
	IntVector3 WorldPosToMapPos(const VECTOR& worldPos)const;
	IntVector3 MapPosToChunkPos(const IntVector3& mapPos) const;
	IntVector3 WorldPosToChunkPos(const VECTOR& worldPos)const;
	VECTOR MapPosToWorldPos(const IntVector3& mapPos)const;
	IntVector3 ChunkPosToMapPos(const IntVector3& chunkPos)const;
	VECTOR ChunkPosToWorldPos(const IntVector3& chunkPos)const;

	bool IsBlock(const IntVector3& mapPos)const;

	float GetBlockSize(void);

	bool IsLoading(void) const { return isLoading_; }

private:

	//描画に必要な情報
	Polygon3DRenderer::PolygonInfo polygonInfo_;
	//ピクセルマテリアル
	std::unique_ptr<Polygon3DMaterial> material_;
	//ポリゴンレンダラー
	std::shared_ptr<Polygon3DRenderer> renderer_;

	// 黒曜石柱の情報
	struct PillarData
	{
		IntVector3 pos;	// 中心座標
		int height;		// 柱の高さ
		bool hasCage;	// 鉄格子付きか
	};

	std::vector<PillarData> pillars_;

	//ロード中か
	bool isLoading_;

	//ロード用スレッド
	std::thread loadThread_;

	int size_;

	int blockImage_;

	JsonInput jsonInput_;

	//プレイヤースポーンポイント
	IntVector3 playerSpawnPoint_;

	//エネミースポーンポイント
	IntVector3 enemySpawnPoint_;

	//ブロックのidと名前の対応表
	std::map<std::string, unsigned short> listNameAndID_;

	//ステージデータ (座標とブロックの種類(ID))
	//std::unordered_map<IntVector3, unsigned short> stageData_;

	//ステージデータチャンク毎
	std::unordered_map<IntVector3, std::shared_ptr<Chunk>> chunkDatas_;

	//ブロックの情報
	std::unique_ptr<BlockInfo> blockInfo_;

	//描画するチャンクをまとめる
	std::vector<IntVector3> drawChanks_;

	//jsonデータをロードする
	void LoadJsonData(void);

	//レンダラーを初期化
	void InitRenderer(void);

	//ステージを作る
	void MakeStage(void);

	//メイン島を生成
	void CreateMainIsland(void);
	//黒曜石柱を生成
	void CreateObsidianPillars(void);
	//エンドクリスタルを配置
	void CreateEndCrystals(void);
	//鉄格子生成
	void CreateIronBarCages(void);
	//プレイヤーのスポーン地点を作る
	void CreatePlayerSpawnPoint(void);
	//中央を作成
	void CreateCenterPoint(void);

	//ブロックを設置する
	void SetBlock(const IntVector3& pos, std::string blockName);
	void SetBlock(const IntVector3& pos, unsigned short blockId);
	//ブロックを削除する
	void DeleteBlock(const IntVector3& pos);

	//ポリゴン情報を更新する
	void UpdatePolygon(void);

	void DrawChankGaid(void);

	void CreateStage(void);
};