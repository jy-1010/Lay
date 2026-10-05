#pragma once
#include "../Common/Vector.h"

class ColorPicker
{
public:

    static constexpr int PREVIEW_HEIGHT = 60;

    static constexpr int SLIDER_WIDTH = 200;
    static constexpr int BAR_HEIGHT = 10;
    static constexpr int BAR_SPACING = 40;
    static constexpr int LABEL_OFFSET_Y = 20;

    static constexpr int SKIN_SIZE = 64;
    static constexpr int PIXEL_SIZE = 8;
    static constexpr int UI_OFFSET_X = 30;
    static constexpr int RGB_Y = 110;
    static constexpr int HSV_Y = 130;
    static constexpr int PREVIEW_Y = 100;

    ColorPicker();

    void Update();
    void Draw();

    IntVector3 GetCurrentColor() const;
    void SetColor(const IntVector3& color);

    void UpdateRGBSlider(int mouseX, int mouseY);
    void UpdateHSVSlider(int mouseX, int mouseY);
    void UpdatePosition(int offsetX, int offsetY);
    void SetHSVPosition(int x, int y);

private:

    // RGB
    IntVector3 rgb_;

    // HSV
    VECTOR hsv_;

    IntVector2 hsvPos_;

    IntVector2 rgbPos_;

    IntVector2 previewPos_;

    IntVector3 currentColor_;
};

