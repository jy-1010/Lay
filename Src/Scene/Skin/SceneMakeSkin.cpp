#include <DxLib.h>
#include <algorithm>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource/ShaderResource.h"
#include "../Manager/Resource/Sound/Sound2DResource.h"
#include "../Manager/Camera.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Player/Skin/SkinManager.h"
#include "../Object/SkinEdit/ColorPicker.h"
#include "../Object/SkinEdit/SkinCanvas.h"
#include "../Object/SkinEdit/SkinRenderer.h"
#include "../Object/SkinEdit/Undo.h"
#include "../Object/SkinEdit/PaintTool.h"
#include "../Object/SkinEdit/FloodFill.h"
#include "../Object/SkinEdit/QuickPalette.h"
#include "../Object/SkinEdit/HSVRing.h"
#include "../Object/SkinEdit/SVArea.h"
#include "../Object/SkinEdit/InputName.h"
#include "SceneMakeSkin.h"

SceneMakeSkin::SceneMakeSkin(void)
{
	auto& resManager = ResourceManager::GetInstance();
	resManager.GetSound2DResource("SkinBGM").lock()->Play();
	previewPlayer_ = std::make_unique<Player>("");
	previewScreen_ = MakeScreen(Application::SCREEN_HALF_X / 2, Application::SCREEN_SIZE_Y, true);
	canvasScreen_ = MakeScreen(SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE, SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE);
	canvasShaderScreen_ = MakeScreen(SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE, SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE);
	saveScreen_ = MakeScreen(SkinCanvas::SIZE, SkinCanvas::SIZE);
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW_ROTATION);
	camera.SetFollow(previewPlayer_->GetTransform().lock());
	colorPicker_ = std::make_shared<ColorPicker>();
	skinCanvas_ = std::make_shared<SkinCanvas>();
	undo_ = std::make_shared<Undo>();
	paintTool_ = std::make_shared<PaintTool>();
	quickPalette_ = std::make_shared<QuickPalette>();
	inputName_ = std::make_shared<InputName>();
	hsvRing_ = std::make_shared<HSVRing>();
	svArea_ = std::make_shared<SVArea>();
	isSave_ = false;
	InitRenderer();
}

SceneMakeSkin::~SceneMakeSkin(void)
{
	auto& resManager = ResourceManager::GetInstance();
	resManager.GetSound2DResource("SkinBGM").lock()->Stop();
}

bool SceneMakeSkin::Init(void)
{
	offset_.x = (Application::SCREEN_SIZE_X - SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE) / 2;

	offset_.y = (Application::SCREEN_SIZE_Y - SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE) / 2;

	int uiX =
		offset_.x +
		SkinCanvas::SIZE *
		SkinRenderer::PIXEL_SIZE +
		UI_OFFSET_X;

	int uiY =
		offset_.y;

	quickPalette_->SetPosition(
		uiX,
		uiY);

	colorPicker_->UpdatePosition(
		offset_.x,
		offset_.y);

	hsvRing_->SetPosition(
		HsvRingPosX,
		HsvRingPosY);

	svArea_->SetPosition(
		SvAreaPosX,
		SvAreaPosY);

	return true;
}

void SceneMakeSkin::Update(void)
{
	auto& keyConfig = KeyConfig::GetInstance();

	if (inputName_->IsUpdate())
	{
		inputName_->Update();
		return;
	}
	if (inputName_->IsEnd())
	{
		if (isSave_)
		{
			return;
		}
		SaveSkin(inputName_->GetInputData());
		isSave_ = true;
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
		return;
	}
	// 更新
	previewPlayer_->Update();
	colorPicker_->Update();
	quickPalette_->Update();

	hsvRing_->Update();

	svArea_->SetHue(
		hsvRing_->GetHue());

	svArea_->Update();

	if (hsvRing_->IsActive() ||
		svArea_->IsActive())
	{
		colorPicker_->SetColor(
			svArea_->GetCurrentColor());
	}

	if (quickPalette_->IsSelected())
	{
		colorPicker_->SetColor(
			quickPalette_->GetSelectedColor());
	}

	// 入力取得
	char keys[256];
	GetHitKeyStateAll(keys);

	int mouseX;
	int mouseY;

	GetMousePoint(&mouseX, &mouseY);

	static bool prevClick = false;
	static bool prevZ = false;

	bool nowClick =
		(GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

	bool clickDown =
		nowClick && !prevClick;

	// ペンとバケツ切り替え
	static bool prevF = false;

	bool nowF =
		CheckHitKey(KEY_INPUT_F) != 0;

	if (nowF && !prevF)
	{
		paintTool_->Toggle();
	}

	prevF = nowF;

	// スキン座標変換
	int x =
		(mouseX - offset_.x) /
		SkinRenderer::PIXEL_SIZE;

	int y =
		(mouseY - offset_.y) /
		SkinRenderer::PIXEL_SIZE;

	bool inCanvas =
		x >= 0 &&
		x < SkinCanvas::SIZE &&
		y >= 0 &&
		y < SkinCanvas::SIZE;

	// Undo保存
	if (paintTool_->IsPen())
	{
		if (clickDown && inCanvas)
		{
			undo_->Push(
				skinCanvas_->GetData());
		}

		if (nowClick && inCanvas)
		{
			skinCanvas_->DrawBrush(
				x,
				y,
				paintTool_->GetSize(),
				colorPicker_->GetCurrentColor());
		}
	}

	// スポイト
	if (GetMouseInput() & MOUSE_INPUT_RIGHT)
	{
		if (x >= 0 &&
			x < SkinCanvas::SIZE &&
			y >= 0 &&
			y < SkinCanvas::SIZE)
		{
			colorPicker_->SetColor(
				skinCanvas_->GetPixel(x, y));
		}
	}

	// バケツ
	if (paintTool_->IsBucket())
	{
		if (clickDown && inCanvas)
		{
			undo_->Push(
				skinCanvas_->GetData());

			FloodFill::Fill(
				skinCanvas_->GetData(),
				x,
				y,
				skinCanvas_->GetPixel(x, y),
				colorPicker_->GetCurrentColor());
		}
	}

	// Ctrl + Z
	bool nowZ =
		keys[KEY_INPUT_Z] != 0;

	bool ctrl =
		keys[KEY_INPUT_LCONTROL] ||
		keys[KEY_INPUT_RCONTROL];

	if (ctrl && nowZ && !prevZ)
	{
		undo_->Pop(
			skinCanvas_->GetData());
	}

	static bool prevQ = false;
	static bool prevE = false;

	bool nowQ = CheckHitKey(KEY_INPUT_Q) != 0;
	bool nowE = CheckHitKey(KEY_INPUT_E) != 0;

	if (nowQ && !prevQ)
	{
		paintTool_->DecreaseSize();
	}

	if (nowE && !prevE)
	{
		paintTool_->IncreaseSize();
	}

	prevQ = nowQ;
	prevE = nowE;

	// 前フレーム保存
	prevClick = nowClick;
	prevZ = nowZ;

	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		inputName_->SetFase(InputName::Fase::FIRST_CHECK);
	}
	UpdateIsPose();
}

void SceneMakeSkin::Draw(void)
{
	colorPicker_->Draw();
	quickPalette_->Draw();
	SetDrawScreen(canvasScreen_);
	ClearDrawScreen();
	SkinRenderer::Draw(*skinCanvas_);

	SetDrawScreen(saveScreen_);
	ClearDrawScreen();
	DrawExtendGraph(0, 0, SkinCanvas::SIZE, SkinCanvas::SIZE, canvasScreen_, false);

	SetDrawScreen(canvasShaderScreen_);
	ClearDrawScreen();
	canvasMaterial_->SetTextureBuf(1, canvasScreen_);
	canvasRenderer_->Draw();

	SetDrawScreen(previewScreen_);
	ClearDrawScreen();
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.GetCamera().CameraSetting();
	previewPlayer_->SetSkinHandle(canvasScreen_);
	previewPlayer_->Draw();

	SetDrawScreen(canvasShaderScreen_);
	SkinRenderer::DrawGrid(SkinRenderer::PIXEL_SIZE, SkinRenderer::GRID_COLOR);

	SetDrawScreen(sceneManager.GetMainScreen());
	DrawGraph(0, 0, previewScreen_,true);
	DrawGraph(offset_.x, offset_.y, canvasShaderScreen_,true);

	hsvRing_->Draw();
	svArea_->Draw();


	DrawString(offset_.x * 2, offset_.y / 4, "Right-click:Dropper", GetColor(255, 255, 255));
	DrawString(offset_.x , offset_.y / 2, "Q/E:Size Change", GetColor(255, 255, 255));

	if (paintTool_->IsPen())
	{
		DrawString(
			offset_.x,
			offset_.y / 4,
			"Mode : PEN (F:Switching)",
			GetColor(255, 255, 255));
	}

	if (paintTool_->IsBucket())
	{
		DrawString(
			offset_.x,
			offset_.y / 4,
			"Mode : BUCKET (F:Switching)",
			GetColor(255, 255, 0));
	}

	int mouseX;
	int mouseY;
	GetMousePoint(&mouseX, &mouseY);

	int x =
		(mouseX - offset_.x) /
		SkinRenderer::PIXEL_SIZE;

	int y =
		(mouseY - offset_.y) /
		SkinRenderer::PIXEL_SIZE;

	if (x >= 0 &&
		x < SkinCanvas::SIZE &&
		y >= 0 &&
		y < SkinCanvas::SIZE)
	{
		IntVector3 color =
			colorPicker_->GetCurrentColor();

		int size =
			paintTool_->GetSize();

		int left =
			offset_.x +
			x * SkinRenderer::PIXEL_SIZE;

		int top =
			offset_.y +
			y * SkinRenderer::PIXEL_SIZE;

		int right =
			left +
			size * SkinRenderer::PIXEL_SIZE;

		int bottom =
			top +
			size * SkinRenderer::PIXEL_SIZE;

		right = std::min(
			right,
			offset_.x + SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE);

		bottom = std::min(
			bottom,
			offset_.y + SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE);

		bool blink =
			(GetNowCount() / flashing) % 2 == 0;

		if (blink)
		{
			DrawBox(
				left,
				top,
				right,
				bottom,
				GetColor(
					color.r,
					color.g,
					color.b),
				FALSE);
		}
	}

	inputName_->Draw();
}

void SceneMakeSkin::Load(void)
{
}

void SceneMakeSkin::SaveSkin(std::string skinName) const
{
	//無限ループで同じ名前で登録されているものがすでにあるかを確認しある場合数字を追加する
	auto& skinManager = SkinManager::GetInstance();
	int i = 0;
	std::string tempName = skinName;
	for (;;)
	{
		if (!skinManager.IsContainName(tempName))
		{
			break;
		}
		tempName = skinName + "_" + std::to_string(i);
		i++;
	}
	SaveDrawValidGraphToPNG(saveScreen_, 0, 0, SkinCanvas::SIZE, SkinCanvas::SIZE , (Application::PATH_IMAGE + "PlayerSkin/" + tempName + ".png").c_str());
	skinManager.SaveSkin(tempName);
}

void SceneMakeSkin::InitRenderer(void)
{
	auto& resManager = ResourceManager::GetInstance();
	int shaderHandle = resManager.GetShaderResource("CanvasPS").lock()->GetHandleId();
	canvasMaterial_ = std::make_shared<Polygon2DMaterial>(shaderHandle,0);
	auto& skinManager = SkinManager::GetInstance();
	int defaultSkin = skinManager.GetHandleId(skinManager.GetDefaultSkinName());
	canvasMaterial_->AddTextureBuf(defaultSkin);
	canvasMaterial_->AddTextureBuf(defaultSkin);
	canvasRenderer_ = std::make_shared<Polygon2DRenderer>(*canvasMaterial_, canvasPolygonInfo_);

	canvasPolygonInfo_.clear();
	auto& vertex = canvasPolygonInfo_.vertex;
	for (int i = 0; i < 4; i++)
	{
		VERTEX2DSHADER ver;
		ver.pos = VGet((i % 2) * (SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE),(i / 2) * (SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE), 0.0f);
		ver.dif = GetColorU8(255, 255, 255, 255);
		ver.rhw = 1.0f;
		ver.u = static_cast<float>(i % 2);
		ver.v = static_cast<float>(i / 2);
		ver.su = ver.pos.x / Application::SCREEN_SIZE_X;
		ver.sv = ver.pos.y / Application::SCREEN_SIZE_Y;
		ver.spc = GetColorU8(0, 0, 0, 0);
		vertex.push_back(ver);
	}
	canvasPolygonInfo_.Indices = { 0,1,2,1,3,2 };
}
