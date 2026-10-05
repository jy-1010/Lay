#include <fstream>
#include "../../Application.h"
#include "../../Utility/JsonUtility.h"
#include "../../Utility/ColorUtility.h"
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../../Manager/Resource/JsonResource.h"
#include "../../Manager/Resource/ImageResource.h"
#include "../../Manager/Resource/ShaderResource.h"
#include "../Item/Block/BlockInfo.h"
#include "../Vertex/VertexInfo.h"
#include "Chunk.h"
#include "Stage.h"

Stage::Stage(void)
{
	//LoadStageData(SelectStageFilePath());
    isLoading_ = true;
    blockInfo_ = std::make_unique<BlockInfo>();
    listNameAndID_ = blockInfo_->GetPairNameAndID();
    size_ = blockInfo_->GetSize();
    LoadJsonData();
    InitRenderer();
    loadThread_ = std::thread(&Stage::CreateStage, this);
}

Stage::~Stage(void)
{
    loadThread_.join();
}

void Stage::Init(void)
{
}

void Stage::Update(void)
{
    if (isLoading_)
    {
        //ロード中
        return;
    }
    for (auto& chunk : chunkDatas_)
    {
        chunk.second->Update();
    }
}

void Stage::Draw(void)
{
    UpdatePolygon();
    renderer_->Draw();
    DrawChankGaid();
}

void Stage::UIDraw(void)
{
    //DrawFormatString(10, 10, 0xffffff, "Polygon %d, Index %d", polygonInfo_.vertex.size(), polygonInfo_.Indices.size());
}

VECTOR Stage::GetPlayerSpawnPos(void)
{
    VECTOR ret;
    ret = IntVector3(jsonInput_.playerSpawnMapPos * blockInfo_->GetSize()).ToVECTOR();
    return ret;
}

VECTOR Stage::GetEnemySpawnPos(void)
{
    VECTOR ret;
    ret = IntVector3(jsonInput_.enemySpawnMapPos * blockInfo_->GetSize()).ToVECTOR();
    return ret;
}

void Stage::LoadJsonData(void)
{
    auto& resManager = ResourceManager::GetInstance();
    auto& json = resManager.GetJsonResource("CreateStageRuleJson").lock()->GetData();
    jsonInput_.radius = json["Radius"];
    jsonInput_.baseHeight = json["BaseHeight"];
    jsonInput_.outHeight = json["OutHeight"];
    for (int i = 0; i < static_cast<int>(json["PillarHeight"].size());i++)
    {
        jsonInput_.pillarHeights.push_back(json["PillarHeight"][i]);
    }
    jsonInput_.pillarNum = json["PillarNum"];
    jsonInput_.pillarHasCageNum = json["PillarHasCageNum"];
    jsonInput_.chanckBlockNum = json["chunkBlockNum"];
    jsonInput_.chankMaxVertexNum = json["chunkMaxVertexNum"];
    jsonInput_.chankMaxIndexNum = json["chunkMaxIndexNum"];
    jsonInput_.playerSpawnMapPos = JsonUtility::GetPosToFloat3D(json["SpawnPosition"]["Player"]);
    jsonInput_.enemySpawnMapPos = JsonUtility::GetPosToFloat3D(json["SpawnPosition"]["Enemy"]);
}

void Stage::InitRenderer(void)
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    auto shaderVSResource = resourceManager.GetShaderResource(blockInfo_->GetShaderInfo().VSKey).lock();
    auto shaderPSResource = resourceManager.GetShaderResource(blockInfo_->GetShaderInfo().PSKey).lock();
    material_ = std::make_unique<Polygon3DMaterial>(shaderVSResource->GetHandleId(), 1, shaderPSResource->GetHandleId(), 1);
    blockImage_ = blockInfo_->GetImageHandle();
    material_->AddTextureBuf(blockImage_);
    renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
}

void Stage::MakeStage(void)
{
    CreateMainIsland();
	CreateObsidianPillars();
	CreateEndCrystals();
	//CreateIronBarCages();
    CreateCenterPoint();
    CreatePlayerSpawnPoint();
}

void Stage::CreateMainIsland(void)
{
    IntVector3 postemp = {};
    const unsigned short endStoneId = listNameAndID_["EndStone"];

    const int radiusSq = jsonInput_.radius * jsonInput_.radius;
    // XZ平面を走査
    for (int x = -jsonInput_.radius; x <= jsonInput_.radius; x++)
    {
        for (int z = -jsonInput_.radius; z <= jsonInput_.radius; z++)
        {
            // 中心からの距離を計算
            //float distance = sqrtf(static_cast<float>(x * x + z * z));
            int distanceSq = x * x + z * z;

            // 半径外なら生成しない
            if (distanceSq > radiusSq)
            {
                continue;
            }
            float rateSq = distanceSq / radiusSq;

            // この地点の地表高さ
            //int height = baseHeight + static_cast<int>(noise * 8);
            int height = jsonInput_.baseHeight -static_cast<int>(rateSq *(jsonInput_.baseHeight - jsonInput_.outHeight));

            postemp.x = x;
            postemp.z = z;
            // 下方向へブロックを配置
            for (int y = 0; y <= height; y++)
            {
                postemp.y = y;
                SetBlock(postemp, endStoneId);
            }
        }
    }
}

void Stage::CreateObsidianPillars(void)
{
    // 柱を配置する円の半径
    constexpr float ringRadius = 36.0f;

    for (int i = 0; i < jsonInput_.pillarNum; i++)
    {
        // 円周上に均等配置
        float angle =
            DX_TWO_PI *
            static_cast<float>(i) /
            jsonInput_.pillarNum;

        int x =static_cast<int>(cosf(angle) * ringRadius);

        int z =static_cast<int>(sinf(angle) * ringRadius);

        // 柱ごとの高さ
        int height = 76 + i * 2;

        // 2本だけ鉄格子付き
        bool hasCage = (i == 2 || i == 7);

        // 情報保存
        pillars_.push_back({{x, 0, z}, height,hasCage});

        constexpr int pillarRadius = 2;

        // 円柱を生成
        for (int px = -pillarRadius;
            px <= pillarRadius;
            px++)
        {
            for (int pz = -pillarRadius;
                pz <= pillarRadius;
                pz++)
            {
                // 円形判定
                if (px * px + pz * pz >
                    pillarRadius * pillarRadius)
                {
                    continue;
                }

                // 高さ方向へ積み上げる
                for (int y = 0; y < height; y++)
                {
                    SetBlock({ x + px, y, z + pz },"Obsidian");
                }
            }
        }
    }
}

void Stage::CreateEndCrystals(void)
{
    // 全ての柱に配置
    for (const auto& pillar : pillars_)
    {
        //CreateEntity(EntityType::END_CRYSTAL,{pillar.pos.x, pillar.height + 1,pillar.pos.z});
    }
}

void Stage::CreateIronBarCages(void)
{
    for (const auto& pillar : pillars_)
    {
        // 鉄格子なしならスキップ
        if (!pillar.hasCage)
        {
            continue;
        }

        int baseY = pillar.height;

        // 柱頂上に鉄格子を生成
        for (int x = -3; x <= 3; x++)
        {
            for (int z = -3; z <= 3; z++)
            {
                // 外周のみ配置
                bool wall =
                    abs(x) == 3 ||
                    abs(z) == 3;

                if (!wall)
                {
                    continue;
                }

                // 高さ3ブロック
                for (int y = 0; y < 3; y++)
                {
                    SetBlock({ pillar.pos.x + x, baseY + y, pillar.pos.z + z }, "IronBar");
                }
            }
        }
    }
}

void Stage::CreatePlayerSpawnPoint()
{
    IntVector3 playerSpawnPos = jsonInput_.playerSpawnMapPos;
    for (int x = -2; x <= 2; x++)
    {
        for (int z = -2; z <= 2; z++)
        {
            SetBlock(playerSpawnPos + IntVector3{x,-1,z}, "Obsidian");
            for (int y = 0; y <= 4; y++)
            {
                DeleteBlock(playerSpawnPos + IntVector3{ x,y,z });
            }
        }
    }
}

void Stage::CreateCenterPoint(void)
{
    for (int x = -2; x <= 2; x++)
    {
        for (int z = -2; z <= 2; z++)
        {
            SetBlock( IntVector3{ x,jsonInput_.baseHeight,z }, "Bedrock");
        }
        SetBlock(IntVector3{ x,jsonInput_.baseHeight + 1,-3 }, "Bedrock");
        SetBlock(IntVector3{ x,jsonInput_.baseHeight + 1,3 }, "Bedrock");
        SetBlock(IntVector3{ -3,jsonInput_.baseHeight + 1,x }, "Bedrock");
        SetBlock(IntVector3{ 3,jsonInput_.baseHeight + 1,-x }, "Bedrock");
    }
    for (int y = 1; y < 5; y++)
    {
        SetBlock(IntVector3{ 0,y + jsonInput_.baseHeight,0 }, "Bedrock");
    }
}

void Stage::SetBlock(const IntVector3& pos, std::string blockName)
{
    unsigned short id = listNameAndID_[blockName];

    SetBlock(pos, id);
}

void Stage::SetBlock(const IntVector3& pos, unsigned short blockId)
{

    //stageData_.insert_or_assign(pos, id);
    IntVector3 chunkPos = MapPosToChunkPos(pos);
    auto it = chunkDatas_.find(chunkPos);
    if (it != chunkDatas_.end())
    {
        //既存の場合
        it->second->AddBlock(pos, blockId);
        return;
    }
    std::shared_ptr<Chunk> data = std::make_shared<Chunk>(chunkPos, *this, *blockInfo_);
    chunkDatas_.emplace(chunkPos,  data);

}

void Stage::DeleteBlock(const IntVector3& pos)
{
    IntVector3 chunkPos = MapPosToChunkPos(pos);

    auto chunkIt = chunkDatas_.find(chunkPos);
    if (chunkIt == chunkDatas_.end())
    {
        return;
    }

    chunkIt->second->DeleteBlock(pos);
}

void Stage::UpdatePolygon(void)
{
    auto& sceneManager = SceneManager::GetInstance();
    int renderDistance = 1;
    auto& camera = sceneManager.GetCamera();
    VECTOR cameraPos = camera.GetPos();
    IntVector3 camerachunk = WorldPosToChunkPos(cameraPos);
    polygonInfo_.clear();
    drawChanks_.clear();
    for (auto& chunkData : chunkDatas_)
    {
        //チャンク座標
        const IntVector3& chunkPos = chunkData.first;
        const IntVector3 sub = chunkPos - camerachunk;
        //描画チャンク処理
        if (abs(sub.x) > renderDistance || abs(sub.y) > renderDistance || abs(sub.z) > renderDistance)
        {
            continue;
        }
        if (CheckCameraViewClip_Box(ChunkPosToWorldPos(chunkPos), ChunkPosToWorldPos(chunkPos + IntVector3{1,1,1})))
        {
            continue;
        }
        //チャンク内の頂点情報
        const auto& inchunkData = chunkData.second;
        if (!inchunkData->IsDraw())
        {
            continue;
        }
        const auto& polygon = inchunkData->GetPolygonInfo();
        int size = static_cast<int>(polygonInfo_.vertex.size());
        auto& vertex = polygon.vertex;
        auto& index = polygon.Indices;
        polygonInfo_.vertex.insert(polygonInfo_.vertex.end(), vertex.begin(), vertex.end());
        for (auto& ind : index)
        {
            polygonInfo_.Indices.push_back(ind + size);
        }
        drawChanks_.push_back(chunkPos);
    }
    //polygonInfo_.Indices = { 0,1,2 };
}

void Stage::DrawChankGaid(void)
{
    VECTOR low;
    VECTOR high;
    for (auto& draw : drawChanks_)
    {
        low = ChunkPosToWorldPos(draw);
        high = ChunkPosToWorldPos(draw + IntVector3{ 1,1,1 });
        DrawCube3D(low, high, ColorUtility::WHITE, ColorUtility::WHITE, false);
    }
}

void Stage::CreateStage(void)
{
    MakeStage();
    Update();

    isLoading_ = false;
}

IntVector3 Stage::WorldPosToMapPos(const VECTOR& worldPos) const
{
    IntVector3 ret;
    ret.x = static_cast<int>(std::floor(worldPos.x / size_));
    ret.y = static_cast<int>(std::floor(worldPos.y / size_));
    ret.z = static_cast<int>(std::floor(worldPos.z / size_));
    return ret;
}

IntVector3 Stage::MapPosToChunkPos(const IntVector3& mapPos) const
{
    IntVector3 ret;
    ret.x = static_cast<int>(std::floor(static_cast<float>(mapPos.x) / jsonInput_.chanckBlockNum));
    ret.y = static_cast<int>(std::floor(static_cast<float>(mapPos.y) / jsonInput_.chanckBlockNum));
    ret.z = static_cast<int>(std::floor(static_cast<float>(mapPos.z) / jsonInput_.chanckBlockNum));
    return ret;
}

IntVector3 Stage::WorldPosToChunkPos(const VECTOR& worldPos) const
{
    return MapPosToChunkPos(WorldPosToMapPos(worldPos));
}

VECTOR Stage::MapPosToWorldPos(const IntVector3& mapPos) const
{
    VECTOR ret;
    ret.x = static_cast<float>(mapPos.x) * size_;
    ret.y = static_cast<float>(mapPos.y) * size_;
    ret.z = static_cast<float>(mapPos.z) * size_;
    return ret;
}

IntVector3 Stage::ChunkPosToMapPos(const IntVector3& chunkPos) const
{
    IntVector3 ret;
    ret.x = static_cast<int>(chunkPos.x * jsonInput_.chanckBlockNum);
    ret.y = static_cast<int>(chunkPos.y * jsonInput_.chanckBlockNum);
    ret.z = static_cast<int>(chunkPos.z * jsonInput_.chanckBlockNum);
    return ret;
}

VECTOR Stage::ChunkPosToWorldPos(const IntVector3& chunkPos) const
{
    return MapPosToWorldPos(ChunkPosToMapPos(chunkPos));
}

bool Stage::IsBlock(const IntVector3& mapPos) const
{
    IntVector3 chunkPos = MapPosToChunkPos(mapPos);
    const auto& it = chunkDatas_.find(chunkPos);
    if (it == chunkDatas_.end())
    {
        return false;
    }
    return it->second->IsBlock(mapPos);
}

float Stage::GetBlockSize(void)
{
    return blockInfo_->GetSize();
}
