#include <fstream>
#include <vector>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource/JsonResource.h"
#include "VertexInfo.h"

Polygon3DRenderer::PolygonInfo VertexInfo::LoadFromFile(const Cube_Param& param)
{
	Polygon3DRenderer::PolygonInfo polygonInfo;
	polygonInfo.clear();
	nlohmann::json jsonfile;
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(param.key).lock();
	jsonfile = jsonResource->GetData();
	// どの面から作るかを読み込み
	std::vector<std::string> Order;
	for (auto& param : jsonfile["Order"])
	{
		Order.push_back(param);
	}
	float size = jsonfile["Size"];
	//頂点情報を面ごとに読み込む
	auto& Vertices = jsonfile["Vertices"];
	for (int i = 0; i < Order.size(); i++)
	{
		//情報を読み込む
		auto& vertex = Vertices[Order[i]];
		UVOffset uvOffset = GetUVOffset(vertex["UVOffset"],param.cubeSize);
		auto inputVertices = GetInputVertices(vertex["Vertices"]);
		//頂点情報を作成
		for (int j = 0;j < inputVertices.size();j++)
		{
			polygonInfo.vertex.push_back(CreateVertex(inputVertices[j], param, uvOffset,size));
		}
		polygonInfo.Indices.push_back(i * 4);
		polygonInfo.Indices.push_back(i * 4 + 1);
		polygonInfo.Indices.push_back(i * 4 + 2);
		polygonInfo.Indices.push_back(i * 4);
		polygonInfo.Indices.push_back(i * 4 + 2);
		polygonInfo.Indices.push_back(i * 4 + 3);
	}
	return  polygonInfo;
}

std::map<std::string, Polygon3DRenderer::PolygonInfo> VertexInfo::LoadFromFileAFace(const Cube_Param& param)
{
	std::map< std::string, Polygon3DRenderer::PolygonInfo> facePolygonInfo;
	nlohmann::json jsonfile;
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(param.key).lock();
	jsonfile = jsonResource->GetData();
	// どの面から作るかを読み込み
	std::vector<std::string> Order;
	for (auto& param : jsonfile["Order"])
	{
		Order.push_back(param);
	}
	float size = jsonfile["Size"];
	//頂点情報を面ごとに読み込む
	auto& Vertices = jsonfile["Vertices"];
	for (int i = 0; i < Order.size(); i++)
	{
		Polygon3DRenderer::PolygonInfo polygonInfo;
		polygonInfo.clear();
		//情報を読み込む
		auto& vertex = Vertices[Order[i]];
		UVOffset uvOffset = GetUVOffset(vertex["UVOffset"], param.cubeSize);
		auto inputVertices = GetInputVertices(vertex["Vertices"]);
		//頂点情報を作成
		for (int j = 0;j < inputVertices.size();j++)
		{
			polygonInfo.vertex.push_back(CreateVertex(inputVertices[j], param, uvOffset, size));
		}
		polygonInfo.Indices.push_back(0);
		polygonInfo.Indices.push_back(1);
		polygonInfo.Indices.push_back(2);
		polygonInfo.Indices.push_back(0);
		polygonInfo.Indices.push_back(2);
		polygonInfo.Indices.push_back(3);
		std::string faseName = Order[i];
		facePolygonInfo.emplace(faseName, polygonInfo);
	}
	return  facePolygonInfo;
}

float VertexInfo::GetPixelSize(std::string key)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(key).lock();
	nlohmann::json jsonfile;
	jsonfile = jsonResource->GetData();

	return jsonfile["Size"];
}

VertexInfo::UVOffset VertexInfo::GetUVOffset(const nlohmann::json offset, VECTOR size)
{
	UVOffset uvOffset;
	uvOffset.leftUp = GetUVOffset(offset, "LeftUp", size);
	uvOffset.rightDown = GetUVOffset(offset, "RightDown", size);
	return uvOffset;
}

FloatVector2 VertexInfo::GetUVOffset(const nlohmann::json offset, const std::string key, VECTOR size)
{
	auto& param = offset[key];
	FloatVector2 uvOffset;
	uvOffset.u = CalcUVOffset(param["u"], size);
	uvOffset.v = CalcUVOffset(param["v"], size);
	return uvOffset;
}

float VertexInfo::CalcUVOffset(const std::string offset, const VECTOR size)
{
	float ret = 0.0f;
	for (auto& param : offset)
	{
		if (param == 'x')
		{
			ret += size.x;
		}
		else if (param == 'y')
		{
			ret += size.y;
		}
		else if (param == 'z')
		{
			ret += size.z;
		}
	}
	return ret;
}

std::vector<VertexInfo::InputVerticesParam> VertexInfo::GetInputVertices(const nlohmann::json vertices)
{
	std::vector<InputVerticesParam> inputVertices;
	for (auto& param : vertices)
	{
		InputVerticesParam vertex;
		auto& pos = param["Pos"];
		vertex.pos = VECTOR(pos["x"], pos["y"], pos["z"]);
		auto& localUV = param["LocalUV"];
		vertex.localUV = FloatVector2(localUV["u"], localUV["v"]);
		auto& normal = param["Normal"];
		vertex.normal = VECTOR(normal["x"], normal["y"], normal["z"]);
		inputVertices.push_back(vertex);
	}
	return inputVertices;
}

VERTEX3DSHADER VertexInfo::CreateVertex(const InputVerticesParam& inputParam, const Cube_Param& cubeParam, const UVOffset& uvoffset,float size)
{
	const VECTOR center = cubeParam.center;
	const VECTOR cubeSize = cubeParam.cubeSize;
	VERTEX3DSHADER ret;
	VECTOR posOffset = VECTOR(cubeSize.x * inputParam.pos.x, cubeSize.y * inputParam.pos.y, cubeSize.z * inputParam.pos.z);
	ret.pos =VScale(VAdd(center, posOffset),size);
	ret.spos = FLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	ret.norm = inputParam.normal;
	ret.tan = VECTOR(0.0f, 0.0f, 0.0f);
	ret.binorm = VECTOR(0.0f, 0.0f, 0.0f);
	ret.dif = COLOR_U8(255, 255, 255, 255);
	ret.spc = COLOR_U8(0, 0, 0, 255);
	float u = ((uvoffset.rightDown.u - uvoffset.leftUp.u) * inputParam.localUV.u + uvoffset.leftUp.u);
	float v = ((uvoffset.rightDown.v - uvoffset.leftUp.v) * inputParam.localUV.v + uvoffset.leftUp.v);
	ret.u = cubeParam.startUV.u + u / cubeParam.TextureSize.u;
	ret.v = cubeParam.startUV.v + v / cubeParam.TextureSize.v;
	ret.su = ret.u;
	ret.sv = ret.v;
	return ret;
}
