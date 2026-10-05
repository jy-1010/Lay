#pragma once
#include "../SceneBase.h"

class Player;

class SceneSelectSkin :  public SceneBase
{
public:

	//1画面に何枚描画するか
	static constexpr int SCREEN_DRAW_NUM = 3;

	//コンストラクタ
	SceneSelectSkin(void);

	//デストラクタ
	~SceneSelectSkin(void)override;

	//初期化処理
	bool Init(void)override;

	//更新処理
	void Update(void)override;

	//描画処理
	void Draw(void)override;

	//ロード処理
	void Load(void)override;

private:

	struct ScreenInfo
	{
		int skinNum = -1;	//スキンの番号
		int screenHandle = -1;	//スクリーンのハンドル
		std::string skinName = "";	//スキンの名前(キー)
		int skinHandle = -1;	//スキンのハンドル
		IntVector2 pos = {0,0};	//表示する座標
		bool isDraw = false;	//表示するか
	};

	static constexpr float SELECT_EDGE_SIZE = 30.0f;

	//スクリーンの大きさ
	IntVector2 screenSize_;

	//スキンそれぞれのプレビュー用スクリーン情報
	std::vector<ScreenInfo>skinPreviewScreens_;

	//選択されているスキンの番号
	int selectSkinNum_;

	//プレイヤー
	std::shared_ptr<Player> previewPlayer_;

	//スクリーンのインフォを追加する
	void AddScreenInfo(std::string skinName);

	//スクリーンを描画するか
	bool IsDrawScren(int skinNum);

	//座標を取得る
	IntVector2 GetScreenPos(int skinNum);

	//選択されているスキンの名前を取得
	std::string GetSelectSkinName(void);
};

