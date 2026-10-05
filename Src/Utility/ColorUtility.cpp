#include <DxLib.h>
#include <cmath>
#include "../../Common/Vector.h"
#include "ColorUtility.h"

IntVector3 ColorUtility::HSVtoRGB(float h, float s, float v)
{
    float c = v * s;
    float x = c * (1 - abs(fmod(h / HUE_SECTION, 2) - 1));
    float m = v - c;

    float r = 0;
    float g = 0;
    float b = 0;

    if (h < HUE_SECTION)
    {
        r = c;
        g = x;
    }
    else if (h < HUE_SECTION * 2)
    {
        r = x;
        g = c;
    }
    else if (h < HUE_SECTION * 3)
    {
        g = c;
        b = x;
    }
    else if (h < HUE_SECTION * 4)
    {
        g = x;
        b = c;
    }
    else if (h < HUE_SECTION * 5)
    {
        r = x;
        b = c;
    }
    else
    {
        r = c;
        b = x;
    }

    IntVector3 color;

    color.r = (int)((r + m) * COLOR_MAX);
    color.g = (int)((g + m) * COLOR_MAX);
    color.b = (int)((b + m) * COLOR_MAX);

    return color;
}

void ColorUtility::RGBtoHSV(IntVector3 color, float& h, float& s, float& v)
{
    float r = color.r / COLOR_MAX_F;
    float g = color.g / COLOR_MAX_F;
    float b = color.b / COLOR_MAX_F;

    float max = r;
    if (g > max) max = g;
    if (b > max) max = b;

    float min = r;
    if (g < min) min = g;
    if (b < min) min = b;

    float diff = max - min;

    v = max;

    if (max == 0)
    {
        s = 0;
    }
    else
    {
        s = diff / max;
    }

    if (diff == 0)
    {
        h = 0;
    }
    else
    {
        if (max == r)
        {
            h = HUE_SECTION * fmod(((g - b) / diff), HUE_RED_OFFSET);
        }
        else if (max == g)
        {
            h = HUE_SECTION * (((b - r) / diff) + HUE_GREEN_OFFSET);
        }
        else
        {
            h = HUE_SECTION * (((r - g) / diff) + HUE_BLUE_OFFSET);
        }

        if (h < 0)
        {
            h += HSV_MAX_HUE;
        }
    }
}

int ColorUtility::ColorChange(VECTOR color)
{
    int ret = 0;
    int r = static_cast<int>(color.x * COLOR_MAX_F);
    int g = static_cast<int>(color.y * COLOR_MAX_F);
    int b = static_cast<int>(color.z * COLOR_MAX_F);
    return ret = (r * 0x010000) + (g * 0x000100) + (b * 0x000001);
}
