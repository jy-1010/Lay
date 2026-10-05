#pragma once

class HSVRing
{
public:

    static constexpr int OUTER_RADIUS = 80;
    static constexpr int INNER_RADIUS = 60;

    HSVRing();

    void Update();
    void Draw();

    void SetPosition(
        int centerX,
        int centerY);

    float GetHue() const;

    bool IsActive()const;

private:

    void CreateRing();

    bool active_;

    float hue_;

    int centerX_;
    int centerY_;

    int ringScreen_;
};

