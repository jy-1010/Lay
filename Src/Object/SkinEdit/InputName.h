#pragma once
#include <string>

class InputName
{
public:

	static constexpr int MARGIN = 30;

	enum class Fase
	{
		NONE,
		FIRST_CHECK,	//これで書くフェーズが終わりでいいか
		INPUT,	//入力
		SECOND_CHECK,	//名前はこれでいいのか
		END	//終了
	};


	//コンストラクタ
	InputName(void);
	//デストラクタ
	~InputName(void);

	//更新処理
	void Update(void);

	//描画処理
	void Draw(void);

	//更新中かを取得
	bool IsUpdate(void) const { return fase_ != Fase::NONE && fase_ != Fase::END; }

	//終了したか
	bool IsEnd(void) const {return fase_ == Fase::END; }

	//フェーズをセットする
	void SetFase(Fase fase) { fase_ = fase; }

	std::string GetInputData(void) { return inputData_; }
private:

	int screen_;

	IntVector2 screenSize_;

	//フェーズ
	Fase fase_;

	//入力された内容
	std::string inputData_;

	//確認用の更新処理
	void CheckUpdate(void);
	//名前の入力
	void InputUpdate(void);
	//確認用の描画処理
	void CheckDraw(void);
	//名前の入力用描画
	void InputDraw(void);
};

