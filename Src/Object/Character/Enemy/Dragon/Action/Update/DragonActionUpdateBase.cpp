#include "DragonActionUpdateBase.h"

DragonActionUpdateBase::DragonActionUpdateBase(std::weak_ptr<Transform> dragonTrans, nlohmann::json json, DragonAction& parent):parent_(parent)
{
	dragonTrans_ = dragonTrans;
	json_ = json;
}

DragonActionUpdateBase::~DragonActionUpdateBase(void)
{
}
