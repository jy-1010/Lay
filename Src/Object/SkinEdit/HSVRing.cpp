#pragma once

#include <DxLib.h>
#include <cmath>
#include "../../Utility/ColorUtility.h"
#include "HSVRing.h"

HSVRing::HSVRing()
{
    hue_ = 0.0f;

    centerX_ = 0;
    centerY_ = 0;

    active_ = false;

    ringScreen_ =
        MakeScreen(
            OUTER_RADIUS * 2,
            OUTER_RADIUS * 2,
            TRUE);

    CreateRing();
}

void HSVRing::Update()
{
    active_ = false;

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

    float dx =
        mouseX - centerX_;

    float dy =
        mouseY - centerY_;

    float distance =
        sqrtf(
            dx * dx +
            dy * dy);

    if (distance < INNER_RADIUS ||
        distance > OUTER_RADIUS)
    {
        return;
    }

    active_ = true;

    float angle =
        atan2f(
            -dy,
            dx);

    if (angle < 0)
    {
        angle += DX_TWO_PI_F;
    }

    hue_ =
        angle /
        DX_TWO_PI_F *
        360.0f;
}

void HSVRing::Draw()
{
    DrawGraph(
        centerX_ - OUTER_RADIUS,
        centerY_ - OUTER_RADIUS,
        ringScreen_,
        TRUE);

    float rad =
        hue_ *
        DX_PI_F /
        180.0f;

    float radius =
        (OUTER_RADIUS +
            INNER_RADIUS) * 0.5f;

    int markerX =
        centerX_ +
        (int)(cosf(rad) * radius);

    int markerY =
        centerY_ -
        (int)(sinf(rad) * radius);

    DrawCircle(
        markerX,
        markerY,
        5,
        GetColor(255, 255, 255),
        TRUE);

    DrawCircle(
        markerX,
        markerY,
        6,
        GetColor(0, 0, 0),
        FALSE);

}

void HSVRing::SetPosition(
    int centerX,
    int centerY)
{
    centerX_ = centerX;
    centerY_ = centerY;
}

float HSVRing::GetHue() const
{
    return hue_;
}

bool HSVRing::IsActive() const
{
    return active_;
}

void HSVRing::CreateRing()
{
    ColorUtility colorUtility;

    SetDrawScreen(ringScreen_);
    ClearDrawScreen();

    for (int y = 0;
        y < OUTER_RADIUS * 2;
        y++)
    {
        for (int x = 0;
            x < OUTER_RADIUS * 2;
            x++)
        {
            float dx =
                x - OUTER_RADIUS;

            float dy =
                y - OUTER_RADIUS;

            float distance =
                sqrtf(
                    dx * dx +
                    dy * dy);

            if (distance < INNER_RADIUS ||
                distance > OUTER_RADIUS)
            {
                continue;
            }

            float angle =
                atan2f(
                    -dy,
                    dx);

            if (angle < 0)
            {
                angle += DX_TWO_PI_F;
            }

            float hue =
                angle /
                DX_TWO_PI_F *
                360.0f;

            IntVector3 rgb =
                colorUtility.HSVtoRGB(
                    hue,
                    1.0f,
                    1.0f);

            DrawPixel(
                x,
                y,
                GetColor(
                    rgb.r,
                    rgb.g,
                    rgb.b));
        }
    }

    SetDrawScreen(
        DX_SCREEN_BACK);
}
