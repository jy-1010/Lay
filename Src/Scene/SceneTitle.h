//#pragma once
#include "SceneBase.h"
#include <vector>

class SceneTitle: public SceneBase
{
public:

	//メンバー関数
	//-----------------------------------------------------------
	SceneTitle(void);

	~SceneTitle(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

	//ロード
	void Load(void)override;

private:

	//ボタンの情報
	std::vector<ButtonInfo> buttonInfo_;

	//ロゴの情報
	std::vector<LogoInfo> logoInfo_;

	//バージョン情報
	std::vector<VirsionInfo> virsionInfo_;

	//最大のレイヤー数
	int maxLayerNum_;

	nlohmann::json json_;

	//現在選択されているもの
	int selectIndex_;

	float time_;

	void LoadJson(void);
	
	void LoadLogo(void);
	void LoadButton(void);
	void LoadVirsion(void);
	DrawStringInfo LoadDrawStringInfo(nlohmann::json stringJson);
	//レイヤーに対応するオブジェクトを取得する
	std::vector<ButtonInfo> GetButtonInfoToLayer(int layerNum);
	std::vector<LogoInfo> GetLogoInfoToLayer(int layerNum);
	std::vector<VirsionInfo> GetVirsionInfoToLayer(int layerNum);

	void ChangeScene(void);

	//ボタンのDraw
	void DrawButton(ButtonInfo button);
	void DrawVirsion(VirsionInfo virsion);

	//マウスと四角形の当たり判定処理と選択変更
	bool ColCheckMouse(bool isCheakMove = true);

	void UpdateSelectIndex(void);
};