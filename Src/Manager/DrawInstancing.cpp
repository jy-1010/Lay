#include <algorithm>
#include "InstanceData.h"
#include "DrawInstancing.h"


DrawInstancing* DrawInstancing::instance_ = nullptr;

DrawInstancing::DrawInstancing(void)
{

}

DrawInstancing::~DrawInstancing(void)
{

}

void DrawInstancing::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DrawInstancing();
	}
	instance_->Init();
}

DrawInstancing& DrawInstancing::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		DrawInstancing::CreateInstance();
	}
	return *instance_;
}

void DrawInstancing::Init(void)
{
}

void DrawInstancing::Draw(void)
{
}

void DrawInstancing::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

void DrawInstancing::AddInstanceData(TYPE type, std::shared_ptr<InstanceData> instanceData)
{
    if (instanceData == nullptr)
    {
        return;
    }

    instanceData_[type].push_back(instanceData);
}

bool DrawInstancing::ChangeType(TYPE oldType, TYPE newType, const std::shared_ptr<InstanceData>& instanceData)
{
    // 変更する必要がない
    if (oldType == newType)
    {
        return false;
    }

    // 旧タイプのデータを検索
    auto oldIt = instanceData_.find(oldType);

    if (oldIt == instanceData_.end())
    {
        return true;
    }

    auto& oldList = oldIt->second;

    // 対象のインスタンスを検索
    auto dataIt = std::find(
        oldList.begin(),
        oldList.end(),
        instanceData
    );

    if (dataIt == oldList.end())
    {
        return false;
    }

    // 新タイプの一覧に追加
    instanceData_[newType].push_back(*dataIt);

    // 旧タイプの一覧から削除
    oldList.erase(dataIt);

    return true;
}
