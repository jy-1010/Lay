#include "PaintTool.h"

PaintTool::PaintTool()
{
    currentTool_ = PEN;
}

void PaintTool::Toggle()
{
    if (currentTool_ == PEN)
    {
        currentTool_ = BUCKET;
    }
    else
    {
        currentTool_ = PEN;
    }
}

bool PaintTool::IsPen() const
{
    return currentTool_ == PEN;
}

bool PaintTool::IsBucket() const
{
    return currentTool_ == BUCKET;
}


int PaintTool::GetSize() const
{
    return size_;
}

void PaintTool::IncreaseSize()
{
    if (size_ < 8)
    {
        size_++;
    }
}

void PaintTool::DecreaseSize()
{
    if (size_ > 1)
    {
        size_--;
    }
}

void PaintTool::SetSize(int size)
{
    if (size < 1)
    {
        size_ = 1;
    }
    else if (size > 8)
    {
        size_ = 8;
    }
    else
    {
        size_ = size;
    }
}