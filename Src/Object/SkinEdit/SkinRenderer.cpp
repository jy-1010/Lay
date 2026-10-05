#include <DxLib.h>
#include "../../Common/Vector.h"
#include "SkinCanvas.h"
#include "SkinRenderer.h"

void SkinRenderer::Draw(
    const SkinCanvas& canvas)
{
    for (int y = 0; y < SkinCanvas::SIZE; y++)
    {
        for (int x = 0; x < SkinCanvas::SIZE; x++)
        {
            IntVector3 color =
                canvas.GetPixel(x, y);

            DrawBox(
                x * PIXEL_SIZE,
                y * PIXEL_SIZE,
                (x + 1) * PIXEL_SIZE,
                (y + 1) * PIXEL_SIZE,
                GetColor(
                    color.r,
                    color.g,
                    color.b),
                TRUE);
        }
    }
}

void SkinRenderer::DrawGrid(
    int pixelSize,
    int gridColor)
{
    for (int i = 0; i <= SkinCanvas::SIZE; i++)
    {
        DrawLine(
            i * pixelSize,
            0,
            i * pixelSize,
            SkinCanvas::SIZE * pixelSize,
            gridColor
        );

        DrawLine(
            0,
            i * pixelSize,
            SkinCanvas::SIZE * pixelSize,
            i * pixelSize,
            gridColor
        );
    }
}