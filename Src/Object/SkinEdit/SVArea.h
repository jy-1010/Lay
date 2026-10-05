#pragma once

class SVArea
{
public:

    static constexpr int SIZE = 70;

    SVArea();

    void Update();
    void Draw();

    void SetPosition(
        int x,
        int y);

    float GetS() const;
    float GetV() const;

    void SetHue(float hue);
    bool IsActive()const;

    IntVector3 GetCurrentColor() const;

private:

    bool active_;

    float s_;
    float v_;

    int posX_;
    int posY_;

    int areaScreen_;

    float hue_ = 0.0f;
    float lastHue_ = -1.0f;
};