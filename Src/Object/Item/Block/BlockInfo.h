#pragma once
#include <string>
#include <vector>
#include <map>
#include "../../../Lib/nlohmann/json.hpp"
#include "../../Renderer/Polygon3DRenderer.h"
#include "../../../Common/Vector.h"

class BlockInfo
{
public:

	//シェーダー情報
	struct ShaderInfo
	{
		std::string VSKey;	// 頂点シェーダーのリソースキー
		std::string PSKey;	// ピクセルシェーダーのリソースキー
	};

	struct Param
	{
		std::string name = "";	//ブロックの名前
		unsigned short id = -1;	//ブロックのID
		float hardness = -1.0f;	//ブロックの硬さ
		float luminous = 0.0f;	//ブロックの発光度
		std::string toolType = "";	//ブロックを破壊するのに適性なツールの種類
		std::vector<std::string> toolLevel;	//ブロックを破壊するのに適性なツールのレベル
		bool isCorrectAnotherTool = false;	//別のツールでも獲得可能かどうか
		FloatVector2 uvOffset = FloatVector2(0.0f, 0.0f);	//ブロックのテクスチャのUVオフセット
		std::map<std::string, Polygon3DRenderer::PolygonInfo> fasesPolygonInfo;	//面ごとの頂点情報
	};

	BlockInfo(void);
	~BlockInfo(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void UIDraw(void);

	const ShaderInfo GetShaderInfo(void) { return shaderInfo_; }

	const int& GetImageHandle(void) const { return imageHandle_; }

	//全ての名前のIDの組み合わせの対応表を送る
	const std::map<std::string, unsigned short> GetPairNameAndID(void);

	//ブロック個別の情報を取得する
	const Param& GetParam(unsigned short id) { return params_.at(id); }
	const Param& GetParam(std::string name);

	const int GetSize(void);
private:

	int imageHandle_;
	FloatVector2 textureSize_;

	ShaderInfo shaderInfo_;
	std::map<unsigned short,Param> params_;
	nlohmann::json blocks;
	void LoadBlockInfo(const std::string blockName);
	std::map<std::string, Polygon3DRenderer::PolygonInfo> MakePolygon(Param param);
};

