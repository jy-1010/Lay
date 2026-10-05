#include <DxLib.h>
#include "../../Common/Vector.h"
#include "../../Utility/ColorUtility.h"
#include "SVArea.h"

SVArea::SVArea()
{
    s_ = 1.0f;
    v_ = 1.0f;

    posX_ = 0;
    posY_ = 0;

    active_ = false;

    areaScreen_ =
        MakeScreen(
            SIZE,
            SIZE,
            TRUE);
}

void SVArea::Update()
{
    active_ = false;

    // Hueが変わったら再生成
    if (hue_ != lastHue_)
    {
        ColorUtility colorUtility;

        SetDrawScreen(areaScreen_);

        for (int py = 0; py < SIZE; py++)
        {
            for (int px = 0; px < SIZE; px++)
            {
                float localS =
                    px / (float)(SIZE - 1);

                float localV =
                    1.0f -
                    (py / (float)(SIZE - 1));

                IntVector3 base =
                    colorUtility.HSVtoRGB(
                        hue_,
                        1.0f,
                        1.0f);

                float pr =
                    255.0f * (1.0f - localS) +
                    base.r * localS;

                float pg =
                    255.0f * (1.0f - localS) +
                    base.g * localS;

                float pb =
                    255.0f * (1.0f - localS) +
                    base.b * localS;

                pr *= localV;
                pg *= localV;
                pb *= localV;

                DrawPixel(
                    px,
                    py,
                    GetColor(
                        (int)pr,
                        (int)pg,
                        (int)pb));
            }
        }

        SetDrawScreen(DX_SCREEN_BACK);

        lastHue_ = hue_;
    }

    // マウス入力
    int mouseX;
    int mouseY;

    GetMousePoint(
        &mouseX,
        &mouseY);

    bool click =
        (GetMouseInput() &
            MOUSE_INPUT_LEFT) != 0;

    // SV選択
    if (click)
    {
        bool inside =
            mouseX >= posX_ &&
            mouseX <= posX_ + SIZE &&
            mouseY >= posY_ &&
            mouseY <= posY_ + SIZE;

        if (inside)
        {
            active_ = true;

            s_ =
                (mouseX - posX_) /
                (float)SIZE;

            v_ =
                1.0f -
                ((mouseY - posY_) /
                    (float)SIZE);
        }
    }
}

void SVArea::Draw()
{
    DrawGraph(
        posX_,
        posY_,
        areaScreen_,
        TRUE);

    DrawBox(
        posX_,
        posY_,
        posX_ + SIZE,
        posY_ + SIZE,
        DxLib::GetColor(255, 255, 255),
        FALSE);
}

void SVArea::SetPosition(
    int x,
    int y)
{
    posX_ = x;
    posY_ = y;
}

float SVArea::GetS() const
{
    return s_;
}

float SVArea::GetV() const
{
    return v_;
}

void SVArea::SetHue(float hue)
{
    hue_ = hue;
}

bool SVArea::IsActive() const
{
    return active_;
}

IntVector3 SVArea::GetCurrentColor() const
{
    ColorUtility colorUtility;

    return colorUtility.HSVtoRGB(
        hue_,
        s_,
        v_);
}
