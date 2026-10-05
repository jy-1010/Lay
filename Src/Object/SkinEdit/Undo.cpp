#include <vector>
#include "../../Common/Vector.h"
#include "Undo.h"

void Undo::Push(IntVector3 skin[64][64])
{
    SkinData snapshot;

    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            snapshot.data[x][y] = skin[x][y];
        }
    }

    undoStack.push_back(snapshot);
}

bool Undo::Pop(IntVector3 skin[64][64])
{
    if (undoStack.empty())
    {
        return false;
    }

    SkinData snapshot = undoStack.back();

    undoStack.pop_back();

    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            skin[x][y] = snapshot.data[x][y];
        }
    }

    return true;
}