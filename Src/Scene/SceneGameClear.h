//#pragma once
#include "SceneBase.h"
#include <vector>
class SceneGameClear : public SceneBase
{
public:

	SceneGameClear(void);
	~SceneGameClear(void)override;

	bool Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Load(void)override;
private:

	//ボタンの情報
	std::vector<ButtonInfo> buttonInfo_;

	//ロゴの情報
	std::vector<LogoInfo> logoInfo_;

	//最大のレイヤー数
	int maxLayerNum_;

	nlohmann::json json_;

	//現在選択されているもの
	int selectIndex_;

	void LoadJson(void);

	void LoadLogo(void);
	void LoadButton(void);
	DrawStringInfo LoadDrawStringInfo(nlohmann::json stringJson);
	//レイヤーに対応するオブジェクトを取得する
	std::vector<ButtonInfo> GetButtonInfoToLayer(int layerNum);
	std::vector<LogoInfo> GetLogoInfoToLayer(int layerNum);

	void ChangeScene(void);

	//ボタンのDraw
	void DrawButton(ButtonInfo button);

	//マウスと四角形の当たり判定処理と選択変更
	bool ColCheckMouse(bool isCheakMove = true);

	void UpdateSelectIndex(void);
};

