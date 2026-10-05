#include <DxLib.h>
#include "../../Common/Vector.h"
#include "ColorPicker.h"
#include "../../Utility/Utility.h"
#include "../../Utility/ColorUtility.h"

ColorPicker::ColorPicker()
{
    rgb_ = {ColorUtility::COLOR_MAX,0,0 };

    currentColor_ = rgb_;

    hsv_ = { 0.0f,1.0f,1.0f };
    ColorUtility colorUtility;

    colorUtility.RGBtoHSV(
        currentColor_,
        hsv_.x,
        hsv_.y,
        hsv_.z);
}

void ColorPicker::Update()
{
    int mouseX;
    int mouseY;

    GetMousePoint(&mouseX, &mouseY);

    bool click =
        (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    if (!click)
    {
        return;
    }

    UpdateRGBSlider(mouseX, mouseY);
    UpdateHSVSlider(mouseX, mouseY);
}

void ColorPicker::Draw()
{
    DrawBox(previewPos_.x, previewPos_.y, previewPos_.x + SLIDER_WIDTH, previewPos_.y + PREVIEW_HEIGHT
        , GetColor(currentColor_.r, currentColor_.g, currentColor_.b), TRUE);

    DrawBox(previewPos_.x, previewPos_.y, previewPos_.x + SLIDER_WIDTH, previewPos_.y + PREVIEW_HEIGHT, ColorUtility::WHITE, FALSE);

    // RED
    DrawString(
        rgbPos_.x,
        rgbPos_.y - LABEL_OFFSET_Y,
        "R",
        ColorUtility::WHITE);

    DrawBox(
        rgbPos_.x,
        rgbPos_.y,
        rgbPos_.x + (int)(rgb_.r / ColorUtility::COLOR_MAX_F * SLIDER_WIDTH),
        rgbPos_.y + BAR_HEIGHT,
        ColorUtility::RED,
        TRUE);

    DrawBox(
        rgbPos_.x,
        rgbPos_.y,
        rgbPos_.x + SLIDER_WIDTH,
        rgbPos_.y + BAR_HEIGHT,
        ColorUtility::WHITE,
        FALSE);

    // GREEN
    DrawString(
        rgbPos_.x,
        rgbPos_.y + BAR_SPACING - LABEL_OFFSET_Y,
        "G",
        ColorUtility::WHITE);

    DrawBox(
        rgbPos_.x,
        rgbPos_.y + BAR_SPACING,
        rgbPos_.x + (int)(rgb_.g / ColorUtility::COLOR_MAX_F * SLIDER_WIDTH),
        rgbPos_.y + BAR_SPACING + BAR_HEIGHT,
        ColorUtility::GREEN,
        TRUE);

    DrawBox(
        rgbPos_.x,
        rgbPos_.y + BAR_SPACING,
        rgbPos_.x + SLIDER_WIDTH,
        rgbPos_.y + BAR_SPACING + BAR_HEIGHT,
        ColorUtility::WHITE,
        FALSE);

    // BLUE
    DrawString(
        rgbPos_.x,
        rgbPos_.y + BAR_SPACING * 2 - LABEL_OFFSET_Y,
        "B",
        ColorUtility::WHITE);

    DrawBox(
        rgbPos_.x,
        rgbPos_.y + BAR_SPACING * 2,
        rgbPos_.x + (int)(rgb_.b / ColorUtility::COLOR_MAX_F * SLIDER_WIDTH),
        rgbPos_.y + BAR_SPACING * 2 + BAR_HEIGHT,
        ColorUtility::BLUE,
        TRUE);

    DrawBox(
        rgbPos_.x,
        rgbPos_.y + BAR_SPACING * 2,
        rgbPos_.x + SLIDER_WIDTH,
        rgbPos_.y + BAR_SPACING * 2 + BAR_HEIGHT,
        ColorUtility::WHITE,
        FALSE);

    // h
    DrawString(
        hsvPos_.x,
        hsvPos_.y - LABEL_OFFSET_Y,
        "H",
        ColorUtility::WHITE);

    DrawBox(
        hsvPos_.x,
        hsvPos_.y,
        hsvPos_.x + (int)((hsv_.x / ColorUtility::HSV_MAX_HUE) * SLIDER_WIDTH),
        hsvPos_.y + BAR_HEIGHT,
        ColorUtility::RED,
        TRUE);

    DrawBox(
        hsvPos_.x,
        hsvPos_.y,
        hsvPos_.x + SLIDER_WIDTH,
        hsvPos_.y + BAR_HEIGHT,
        ColorUtility::WHITE,
        FALSE);

    // s
    DrawString(
        hsvPos_.x,
        hsvPos_.y + BAR_SPACING - LABEL_OFFSET_Y,
        "S",
        ColorUtility::WHITE);

    DrawBox(
        hsvPos_.x,
        hsvPos_.y + BAR_SPACING,
        hsvPos_.x + (int)(hsv_.y * SLIDER_WIDTH),
        hsvPos_.y + BAR_SPACING + BAR_HEIGHT,
        ColorUtility::GREEN,
        TRUE);

    DrawBox(
        hsvPos_.x,
        hsvPos_.y + BAR_SPACING,
        hsvPos_.x + SLIDER_WIDTH,
        hsvPos_.y + BAR_SPACING + BAR_HEIGHT,
        ColorUtility::WHITE,
        FALSE);

    // v
    DrawString(
        hsvPos_.x,
        hsvPos_.y + BAR_SPACING * 2 - LABEL_OFFSET_Y,
        "V",
        ColorUtility::WHITE);

    DrawBox(
        hsvPos_.x,
        hsvPos_.y + BAR_SPACING * 2,
        hsvPos_.x + (int)(hsv_.z * SLIDER_WIDTH),
        hsvPos_.y + BAR_SPACING * 2 + BAR_HEIGHT,
        ColorUtility::BLUE,
        TRUE);

    DrawBox(
        hsvPos_.x,
        hsvPos_.y + BAR_SPACING * 2,
        hsvPos_.x + SLIDER_WIDTH,
        hsvPos_.y + BAR_SPACING * 2 + BAR_HEIGHT,
        ColorUtility::WHITE,
        FALSE);
}

IntVector3 ColorPicker::GetCurrentColor() const
{
    return currentColor_;
}

void ColorPicker::SetColor(const IntVector3& color)
{
    currentColor_ = color;

    rgb_ = color;

    //ColorUtility colorUtility;

    ColorUtility::RGBtoHSV(
        currentColor_,
        hsv_.x,
        hsv_.y,
        hsv_.z);
}

void ColorPicker::UpdateRGBSlider(int mouseX, int mouseY)
{
    if (mouseX < rgbPos_.x ||
        mouseX > rgbPos_.x + SLIDER_WIDTH)
    {
        return;
    }

    // R
    if (mouseY >= rgbPos_.y &&
        mouseY <= rgbPos_.y + BAR_HEIGHT)
    {
        rgb_.r =
            (int)(((mouseX - rgbPos_.x) /
                (float)SLIDER_WIDTH) * ColorUtility::COLOR_MAX);

        if (rgb_.r < 0) rgb_.r = 0;
        if (rgb_.r > ColorUtility::COLOR_MAX) rgb_.r = ColorUtility::COLOR_MAX;
    }

    // G
    else if (
        mouseY >= rgbPos_.y + BAR_SPACING &&
        mouseY <= rgbPos_.y + BAR_SPACING + BAR_HEIGHT)
    {
        rgb_.g =
            (int)(((mouseX - rgbPos_.x) /
                (float)SLIDER_WIDTH) * ColorUtility::COLOR_MAX);

        if (rgb_.g < 0) rgb_.g = 0;
        if (rgb_.g > ColorUtility::COLOR_MAX) rgb_.g = ColorUtility::COLOR_MAX;
    }

    // B
    else if (
        mouseY >= rgbPos_.y + BAR_SPACING * 2 &&
        mouseY <= rgbPos_.y + BAR_SPACING * 2 + BAR_HEIGHT)
    {
        rgb_.b =
            (int)(((mouseX - rgbPos_.x) /
                (float)SLIDER_WIDTH) * ColorUtility::COLOR_MAX);

        if (rgb_.b < 0) rgb_.b = 0;
        if (rgb_.b > ColorUtility::COLOR_MAX) rgb_.b = ColorUtility::COLOR_MAX;
    }

 

    currentColor_ = rgb_;

    ColorUtility colorUtility;

    colorUtility.RGBtoHSV(
        currentColor_,
        hsv_.x,
        hsv_.y,
        hsv_.z);
}

void ColorPicker::UpdateHSVSlider(int mouseX, int mouseY)
{
    if (mouseX < hsvPos_.x ||
        mouseX > hsvPos_.x + SLIDER_WIDTH)
    {
        return;
    }

    // H
    if (mouseY >= hsvPos_.y &&
        mouseY <= hsvPos_.y + BAR_HEIGHT)
    {
        hsv_.x =
            ((mouseX - hsvPos_.x) /
                (float)SLIDER_WIDTH) * ColorUtility::HSV_MAX_HUE;
    }

    // S
    else if (
        mouseY >= hsvPos_.y + BAR_SPACING &&
        mouseY <= hsvPos_.y + BAR_SPACING + BAR_HEIGHT)
    {
        hsv_.y =
            (mouseX - hsvPos_.x) /
            (float)SLIDER_WIDTH;

        if (hsv_.y < 0) hsv_.y = 0;
        if (hsv_.y > 1) hsv_.y = 1;
    }

    // V
    else if (
        mouseY >= hsvPos_.y + BAR_SPACING * 2 &&
        mouseY <= hsvPos_.y + BAR_SPACING * 2 + BAR_HEIGHT)
    {
        hsv_.z =
            (mouseX - hsvPos_.x) /
            (float)SLIDER_WIDTH;

        if (hsv_.z < 0) hsv_.z = 0;
        if (hsv_.z > 1) hsv_.z = 1;
    }

    ColorUtility colorUtility;

    currentColor_ =
        colorUtility.HSVtoRGB(
            hsv_.x,
            hsv_.y,
            hsv_.z);

    rgb_ = currentColor_;
}

void ColorPicker::UpdatePosition(int offsetX, int offsetY)
{
    int uiX = offsetX + SKIN_SIZE * PIXEL_SIZE + UI_OFFSET_X;
    int uiY = offsetY;

    rgbPos_.x = uiX;
    rgbPos_.y = uiY + RGB_Y;

    hsvPos_.x = rgbPos_.x;
    hsvPos_.y = rgbPos_.y + HSV_Y;

    previewPos_.x = uiX;
    previewPos_.y = hsvPos_.y + PREVIEW_Y;
}

void ColorPicker::SetHSVPosition(int x, int y)
{
    hsvPos_.x = x;
    hsvPos_.y = y;
}

