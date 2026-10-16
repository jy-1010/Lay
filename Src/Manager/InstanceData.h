#pragma once
class InstanceData
{
public:

    //GPUに送るインスタンスデータ
    struct Data
    {
        float position[4]; // xyz = 位置、w = 1
        float rotation[4]; // xyz = 回転角（ラジアン）
        float scale[4];    // xyz = 拡大率、w = 1
    };

    static_assert(sizeof(Data) == 48);

	//Data構造体の参照を保持する(変更をすぐに反映するため)
    InstanceData(Data&);
	~InstanceData(void);

	void Kill(void){isDead_ = true;}

private:

    bool isDead_;
	Data& data_;
};

