#pragma once
#include "../Common/Vector.h"

class ColorUtility
{
public:

    //定数
     
    //カラーコード
    static constexpr int BLACK = 0x000000;	//黒
    static constexpr int RED = 0xFF0000;	//赤
    static constexpr int BLUE = 0x0000FF;	//青
    static constexpr int YELLOW = 0xFFFF00;	//黄色
    static constexpr int GREEN = 0x008000;	//緑
    static constexpr int CYAN = 0x00FFFF;	//水色
    static constexpr int PINK = 0xFFC0CB;	//桃色
    static constexpr int ORANGE = 0xFFA500;	//オレンジ
    static constexpr int LIME = 0xADFF2F;	//黄緑
    static constexpr int PURPLE = 0x800080;	//紫
    static constexpr int WHITE = 0xFFFFFF;	//白
    static constexpr int GRAY = 0xBBBBBB;	//灰色
    static constexpr int BROWN = 0x8B4513;	//茶色 
    
    // RGBの最大値（0～255）
    static constexpr int COLOR_MAX = 255;

    // RGB計算用のfloat版最大値
    static constexpr float COLOR_MAX_F = 255.0f;

    // HSVの1セクションの角度（360° ÷ 6）
    static constexpr float HUE_SECTION = 60.0f;

    // 色相(Hue)の最大値
    static constexpr float HSV_MAX_HUE = 360.0f;

    // RGB→HSV変換時の色相補正値
    // Red領域 : 0～60°
    static constexpr float HUE_RED_OFFSET = 6.0f;

    // Green領域 : 120～180°
    static constexpr float HUE_GREEN_OFFSET = 2.0f;

    // Blue領域 : 240～300°
    static constexpr float HUE_BLUE_OFFSET = 4.0f;

    static IntVector3 HSVtoRGB(float h, float s, float v);
    static void RGBtoHSV(IntVector3 color, float& h, float& s, float& v);

    //色の割合をintに変換する VECTOR 0～1
    static int ColorChange(VECTOR color);
};
