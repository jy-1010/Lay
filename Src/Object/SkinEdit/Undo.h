#pragma once

struct SkinData
{
    IntVector3 data[64][64];
};

class Undo
{
public:

    void Push(IntVector3 skin[64][64]);

    bool Pop(IntVector3 skin[64][64]);

private:

    std::vector<SkinData> undoStack;
};

