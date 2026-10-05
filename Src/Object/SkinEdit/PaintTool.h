#pragma once

class PaintTool
{
public:

    enum TOOL
    {
        PEN,
        BUCKET
    };

public:

    PaintTool();

    void Toggle();

    bool IsPen() const;
    bool IsBucket() const;

    int GetSize() const;
    void SetSize(int size);

    void IncreaseSize();
    void DecreaseSize();

private:

    int size_ = 1;

    TOOL currentTool_;
};