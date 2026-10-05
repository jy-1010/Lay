#include <DxLib.h>
#include "../../Common/Vector.h"
#include "../../Utility/Utility.h"
#include "../../Utility/ColorUtility.h"
#include "QuickPalette.h"

QuickPalette::QuickPalette()
{
    colors_[0] = { 255,   0,   0 };   // Red
    colors_[1] = { 255, 128,   0 };   // Orange
    colors_[2] = { 255, 255,   0 };   // Yellow
    colors_[3] = { 128, 255,   0 };   // YellowGreen
    colors_[4] = { 0, 255,   0 };   // Green
    colors_[5] = { 0, 255, 128 };   // Mint
    colors_[6] = { 0, 255, 255 };   // Cyan
    colors_[7] = { 0, 128, 255 };   // SkyBlue
    colors_[8] = { 0,   0, 255 };   // Blue
    colors_[9] = { 128,   0, 255 };   // Purple
    colors_[10] = { 255,   0, 255 };   // Magenta
    colors_[11] = { 255,   0, 128 };   // Pink
    colors_[12] = { 255, 255, 255 };   // White
    colors_[13] = { 0,   0,   0 };   // Black
}

void QuickPalette::SetPosition(int x,int y)
{
    pos_ = { x,y };
}

void QuickPalette::Update()
{
    selected_ = false;

    int mouseX;
    int mouseY;

    GetMousePoint(
        &mouseX,
        &mouseY);

    bool click =
        (GetMouseInput() &
            MOUSE_INPUT_LEFT) != 0;

    if (!click)
    {
        return;
    }

    for (int i = 0; i < COUNT; i++)
    {
        int px =
            pos_.x +
            (i % COLUMNS) *
            (SIZE + SPACING);

        int py =
            pos_.y +
            (i / COLUMNS) *
            (SIZE + SPACING);

        if (mouseX >= px &&
            mouseX <= px + SIZE &&
            mouseY >= py &&
            mouseY <= py + SIZE)
        {
            selectedColor_ =
                colors_[i];

            selected_ = true;

            return;
        }
    }
}

void QuickPalette::Draw()
{
    for (int i = 0; i < COUNT; i++)
    {
        auto c = colors_[i];

        int px =
            pos_.x +
            (i % COLUMNS) *
            (SIZE + SPACING);

        int py =
            pos_.y +
            (i / COLUMNS) *
            (SIZE + SPACING);

        DrawBox(
            px,
            py,
            px + SIZE,
            py + SIZE,
            GetColor(
                c.r,
                c.g,
                c.b),
            TRUE);

        DrawBox(
            px,
            py,
            px + SIZE,
            py + SIZE,
            ColorUtility::WHITE,
            FALSE);
    }
}

bool QuickPalette::IsSelected() const
{
    return selected_;
}

IntVector3 QuickPalette::GetSelectedColor() const
{
    return selectedColor_;
}

bool QuickPalette::IsClicked() const
{
    return clicked_;
}
