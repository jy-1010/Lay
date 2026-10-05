#pragma once

class SkinCanvas
{
public:

    static constexpr int SIZE = 64;

    SkinCanvas();

    void Clear(IntVector3 color);

    void DrawBrush(
        int centerX,
        int centerY,
        int size,
        IntVector3 color);

    void SetPixel(
        int x,
        int y,
        IntVector3 color);

    IntVector3 GetPixel(

        int x,
        int y) const;

    IntVector3(&GetData())[SIZE][SIZE];

private:

    IntVector3 pixels_[SIZE][SIZE];
};

