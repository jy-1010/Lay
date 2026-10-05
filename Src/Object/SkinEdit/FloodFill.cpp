#include <queue>
#include "../../Common/Vector.h"
#include "FloodFill.h"

void FloodFill::Fill(
    IntVector3 skin[ColorPicker::SKIN_SIZE][ColorPicker::SKIN_SIZE],
    int startX,
    int startY,
    IntVector3 target,
    IntVector3 replacement)
{
    if (target == replacement)
    {
        return;
    }

    if (startX < 0 || startX >= ColorPicker::SKIN_SIZE ||
        startY < 0 || startY >= ColorPicker::SKIN_SIZE)
    {
        return;
    }

    if (skin[startX][startY] != target)
    {
        return;
    }

    std::queue<std::pair<int, int>> q;
    q.push({ startX, startY });

    while (!q.empty())
    {
        auto [x, y] = q.front();
        q.pop();

        if (x < 0 || x >= ColorPicker::SKIN_SIZE ||
            y < 0 || y >= ColorPicker::SKIN_SIZE)
        {
            continue;
        }

        if (skin[x][y] != target)
        {
            continue;
        }

        skin[x][y] = replacement;

        q.push({ x + 1, y });
        q.push({ x - 1, y });
        q.push({ x, y + 1 });
        q.push({ x, y - 1 });
    }
}