#pragma once
#include "Resource.h"
class ShaderResource : public Resource
{
public:

	enum class SHADER_TYPE
	{
		NONE,
		VERTEX, // 頂点シェーダー
		PIXEL,  // ピクセルシェーダー
	};

	// コンストラクタ
	ShaderResource(void) = delete;
	ShaderResource(nlohmann::json json);
	// デストラクタ
	~ShaderResource(void)override;
	// ロード
	bool Load(void) override;
	// ロードされているか
	bool IsLoaded(void) const override;

	// ハンドルIDを取得する
	const int GetHandleId(void) const { return handleId_; }

	// シェーダーの種類を取得する
	const SHADER_TYPE GetShaderType(void) const { return shaderType_; }

private:
	int handleId_; // シェーダーのハンドルID
	SHADER_TYPE shaderType_; // シェーダーの種類
	// シェーダーの情報をロード
	void LoadResourceInfo(void) override;
	// シェーダーの完全初期化
	void Init(void) override;
};

