#include "../Item/Block/BlockInfo.h"
#include "Chunk.h"

Chunk::Chunk(IntVector3 chunkPos, Stage& stage,BlockInfo& blockInfo):stage_(stage),blockInfo_(blockInfo)
{
    isDraw_ = false;
	chunkPos_ = chunkPos;
    chunkCube_.low = stage_.ChunkPosToMapPos(chunkPos_);
    chunkCube_.high = stage_.ChunkPosToMapPos(chunkPos_ + IntVector3(1,1,1));
    isUpdate_ = false;
    polygonInfo_.vertex.reserve(stage_.GetJsonInput().chankMaxVertexNum);
    polygonInfo_.Indices.reserve(stage_.GetJsonInput().chankMaxIndexNum);
    InitCheckList();
}

Chunk::~Chunk(void)
{
	polygonInfo_.clear();
	mapData_.clear();
    faceChackList_.clear();
}

void Chunk::Update(void)
{
    if (isUpdate_)
    {
        UpdatePolygonInfo();
        isUpdate_ = false;
    }
}

void Chunk::AddBlock(IntVector3 mapPos, unsigned short id)
{
	mapData_.insert_or_assign(mapPos,id);
    inBlockID_.insert(id);
    isUpdate_ = true;
}

void Chunk::DeleteBlock(IntVector3 mapPos)
{
	mapData_.erase(mapPos);
    isUpdate_ = true;
}

bool Chunk::IsBlock(const IntVector3& mapPos)
{
    const auto& it = mapData_.find(mapPos);
    if (it != mapData_.end())
    {
        return true;
    }
    return false;
}

void Chunk::InitCheckList(void)
{
    faceChackList_.emplace("Left", FaceCheck{ {-1,0,0},1 << 0 });
    faceChackList_.emplace("Right", FaceCheck{ {1,0,0},1 << 1 });
    faceChackList_.emplace("Bottom", FaceCheck{ {0,-1,0},1 << 2 });
    faceChackList_.emplace("Top", FaceCheck{ {0,1,0},1 << 3 });
    faceChackList_.emplace("Back", FaceCheck{ {0,0,-1},1 << 4 });
    faceChackList_.emplace("Front", FaceCheck{ {0,0,1},1 << 5 });
}

void Chunk::UpdatePolygonInfo(void)
{
    polygonInfo_.clear();
    std::map<unsigned short, BlockInfo::Param> params;
    
    for (auto& id : inBlockID_)
    {
        const auto& param = blockInfo_.GetParam(id);
        params.emplace(id, param);
    }
    VECTOR worldPos;
    IntVector3 tempPos;
    for (auto& mapData : mapData_)
    {
        //uint8_tでどこを描画するかを表す 1 = left,2 = right,4 = bottom,8 = top, 16 = back,32 = front
        uint8_t isDraw = 0;

        const IntVector3& mapPos = mapData.first;
        //隣接する場所にブロックがあるかを判定しなければその面を描画するため頂点を生成する
        for (auto& chack : faceChackList_)
        {
            tempPos = mapPos + chack.second.offSet;
            if (!chunkCube_.InCube(tempPos))
            {
                if (!stage_.IsBlock(tempPos))
                {
                    isDraw |= chack.second.flag;
                }
            }
            //else if (!mapData_.contains(mapPos + chack.second.offSet))
            else if (!IsBlock(tempPos))
            {
                isDraw |= chack.second.flag;
            }
        }
        if (isDraw == 0)
        {
            continue;
        }
        auto& param = params.at(mapData.second);
        worldPos = stage_.MapPosToWorldPos(mapPos);
        for (auto& chack : faceChackList_)
        {
            if ((chack.second.flag & isDraw) == 0)
            {
                continue;
            }
            int size = static_cast<int>(polygonInfo_.vertex.size());
            auto facePolygon = param.fasesPolygonInfo.find(chack.first);
            auto& vertex = facePolygon->second.vertex;
            auto& index = facePolygon->second.Indices;
            for (auto& ver : vertex)
            {
                VERTEX3DSHADER vertemp = ver;
                vertemp.pos = VAdd(ver.pos, worldPos);
                polygonInfo_.vertex.push_back(vertemp);
            }
            for (auto& ind : index)
            {
                polygonInfo_.Indices.push_back(ind + size);
            }
        }
    }
    if (static_cast<int>(polygonInfo_.vertex.size()) > 0)
    {
        isDraw_ = true;
    }
    else
    {
        isDraw_ = false;
    }
}

bool Chunk::ChunkCube::InCube(const IntVector3& mapPos)
{
    if (mapPos.x >= low.x && mapPos.y >= low.y && mapPos.z >= low.z &&
        mapPos.x < high.x && mapPos.y < high.y && mapPos.z < high.z)
    {
        return true;
    }
    return false;
}
