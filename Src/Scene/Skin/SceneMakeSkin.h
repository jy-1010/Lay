#pragma once
#include "../SceneBase.h"
#include "../../Common/Vector.h"
#include "../../Renderer/Polygon2DRenderer.h"

class Player;
class ColorPicker;
class SkinCanvas;
class Undo;
class PaintTool;
class QuickPalette;
class HSVRing;
class SVArea;
class InputName;

class SceneMakeSkin : public SceneBase
{
public:
    SceneMakeSkin(void);
	~SceneMakeSkin(void)override;
    
	bool Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Load(void)override;

private:

	static constexpr int UI_OFFSET_X = 30;

	static constexpr int HsvRingPosX = 900;
	static constexpr int HsvRingPosY = 552;
	static constexpr int SvAreaPosX = 867;
	static constexpr int SvAreaPosY = 518;

	static constexpr int flashing = 400;

	std::shared_ptr<Polygon2DMaterial>canvasMaterial_;
	std::shared_ptr<Polygon2DRenderer>canvasRenderer_;
	Polygon2DRenderer::PolygonInfo canvasPolygonInfo_;

	std::shared_ptr<ColorPicker> colorPicker_;
	std::shared_ptr<SkinCanvas> skinCanvas_;
	std::shared_ptr<Undo> undo_;
	std::shared_ptr<PaintTool> paintTool_;
	std::shared_ptr<QuickPalette> quickPalette_;
	std::shared_ptr<InputName> inputName_;
	std::shared_ptr<HSVRing> hsvRing_;
	std::shared_ptr<SVArea> svArea_;

	std::unique_ptr<Player> previewPlayer_;
	//プレイヤーを見るためのスクリーン
	int previewScreen_;

	//キャンバス用スクリーン
	int canvasScreen_;

	//キャンバスシェーダー用
	int canvasShaderScreen_;

	//保存用スクリーン
	int saveScreen_;

	//描画位置
	IntVector2 offset_;

	//保存したか
	bool isSave_;

	//スキンを保存する
	void SaveSkin(std::string skinName) const;

	//レンダラーの初期化
	void InitRenderer(void);
};