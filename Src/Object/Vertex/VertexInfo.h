#pragma once
#include <string>
#include <vector>
#include "../../Lib/nlohmann/json.hpp"
#include "../../Renderer/Polygon3DRenderer.h"

class VertexInfo
{
public:

	//UV
	struct UVOffset
	{
		FloatVector2 leftUp;	//左上
		FloatVector2 rightDown;	//右下
	};

	//キューブの情報
	struct Cube_Param
	{
		std::string key;	//キー
		VECTOR center;		//中央
		VECTOR cubeSize;	//キューブの大きさ
		FloatVector2 TextureSize;	//テクスチャの大きさ
		FloatVector2 startUV;	//UV展開を始める位置
	};

	struct InputVerticesParam
	{
		VECTOR pos;
		FloatVector2 localUV;
		VECTOR normal;
	};

	//ファイルを用いて頂点情報を取得する
	static Polygon3DRenderer::PolygonInfo LoadFromFile(const Cube_Param& param);
	//ファイルを用いて頂点情報を面ごとに取得する
	static std::map<std::string, Polygon3DRenderer::PolygonInfo> LoadFromFileAFace(const Cube_Param& param);
	static float GetPixelSize(std::string key);
private:
	static UVOffset GetUVOffset(const nlohmann::json offset,VECTOR size);
	static FloatVector2 GetUVOffset(const nlohmann::json offset,const std::string key,VECTOR size);
	static float CalcUVOffset(const std::string offset, const VECTOR size);
	static std::vector<InputVerticesParam> GetInputVertices(const nlohmann::json vertices);
	static VERTEX3DSHADER CreateVertex(const InputVerticesParam& inputParam, const Cube_Param& cubeParam,const UVOffset& uvoffset,float size);
};

