#pragma once
#include "../../ObjectBase.h"
class ToolBase : public ObjectBase
{
public:
	struct Param
	{
		std::string type;	//ツールの種類
		std::string level;	//ツールのレベル
		float durability;	//ツールの耐久値
		std::string specialAction;	//ツールの特殊アクション
	};

	ToolBase(void);
	~ToolBase(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UIDraw(void) override;

	const Param& GetParam(void) { return param_; }

protected:
	Param param_;

private:
};

