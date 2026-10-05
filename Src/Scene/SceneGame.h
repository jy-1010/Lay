#pragma once
#include"SceneBase.h"

class Stage;
class Player;
class Dragon;

class SceneGame :public SceneBase
{
public:
	//メンバー関数
	//-----------------------------------------------------------
	//初期化処理(初回の1度のみ実行される)
	SceneGame(void) ;

	virtual ~SceneGame(void) override;

	virtual bool Init(void) override;

	//更新処理
	virtual void Update(void) override;

	//描画処理
	virtual void Draw(void) override;

	void DebugDraw(void);

	float GetBlockSize(void);

	//座標変換
	IntVector3 WorldPosToMapPos(const VECTOR& worldPos)const;
	IntVector3 MapPosToChunkPos(const IntVector3& mapPos) const;
	IntVector3 WorldPosToChunkPos(const VECTOR& worldPos)const;
	VECTOR MapPosToWorldPos(const IntVector3& mapPos)const;
	IntVector3 ChunkPosToMapPos(const IntVector3& chunkPos)const;
	VECTOR ChunkPosToWorldPos(const IntVector3& chunkPos)const;

	bool IsBlock(const IntVector3& mapPos);

protected:
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Dragon> dragon_;
};