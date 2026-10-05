#pragma once

class SkinCanvas;

class SkinRenderer
{
public:

    static constexpr int PIXEL_SIZE = 8;
    static constexpr int GRID_COLOR = 0xC8C8C8;

    static void Draw(
        const SkinCanvas& canvas);

    static void DrawGrid(
        int pixelSize,
        int gridColor);

private:
};

