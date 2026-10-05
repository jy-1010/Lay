#pragma once
#include <map>
#include <set>
#include <vector>
#include "../../Common/Vector.h"
#include "Stage.h"

class BlockInfo;

class Chunk
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="chunkPos">チャンク座標</param>
	/// <param name="stage">ステージ親クラス</param>
	/// <param name="blockInfo">ブロック情報クラス</param>
	Chunk(IntVector3 chunkPos,Stage& stage, BlockInfo& blockInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Chunk(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// ブロックを追加する
	/// </summary>
	/// <param name="mapPos">追加する場所のマップ座標</param>
	/// <param name="id">ブロックのid</param>
	void AddBlock(IntVector3 mapPos, unsigned short id);

	/// <summary>
	/// ブロックを削除
	/// </summary>
	/// <param name="mapPos">削除するブロックのマップ座標</param>
	void DeleteBlock(IntVector3 mapPos);

	/// <summary>
	/// ポリゴン情報を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Polygon3DRenderer::PolygonInfo GetPolygonInfo(void) const { return polygonInfo_; }

	/// <summary>
	/// 指定したマップ座標にブロックがあるか
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <returns></returns>
	bool IsBlock(const IntVector3& mapPos);

	/// <summary>
	/// 描画する箇所があるか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsDraw(void) const { return isDraw_; }

private:

	//隣接ブロック確認用
	struct FaceCheck
	{
		IntVector3 offSet = {};
		uint8_t flag = 0;
	};

	struct ChunkCube
	{
		IntVector3 low = {};//原点に近いほう
		IntVector3 high = {};//原点に遠いほう
		bool InCube(const IntVector3& mapPos);
	};

	//ステージ親クラス
	Stage& stage_;

	//ブロック情報
	BlockInfo& blockInfo_;

	//自分のチャンク
	IntVector3 chunkPos_;

	//自分のチャンクの範囲
	ChunkCube chunkCube_;

	//マップのデータ
	std::unordered_map<IntVector3, unsigned short>mapData_;

	//チャンクに含まれているブロックの種類
	std::set<unsigned short> inBlockID_;

	//ポリゴン情報
	Polygon3DRenderer::PolygonInfo polygonInfo_;

	//隣にブロックがあるか判定するときに使うもの
	std::map<std::string, FaceCheck> faceChackList_;

	//更新が必要か
	bool isUpdate_;

	//チェック用のものを初期化
	void InitCheckList(void);

	//ポリゴン情報の更新
	void UpdatePolygonInfo(void);

	//描画する箇所があるか
	bool isDraw_;
};

