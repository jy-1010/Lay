#pragma once
#include "../../Common/Vector.h"

class QuickPalette
{
public:

    static constexpr int COUNT = 14;

    static constexpr int SIZE = 24;
    static constexpr int SPACING = 5;
    static constexpr int COLUMNS = 7;

    QuickPalette();

    void Update();
    void Draw();

    void SetPosition(int x, int y);

    bool IsSelected() const;
  
    IntVector3 GetSelectedColor() const;
    bool IsClicked() const;

private:

    IntVector3 colors_[COUNT];

    IntVector2 pos_;

    bool selected_ = false;
    bool clicked_ = false;
    IntVector3 selectedColor_;
};
