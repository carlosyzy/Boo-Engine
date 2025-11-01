#pragma once
#include <string>
class Vec2
{
private:
    float _x;
    float _y;

public:
    Vec2();
    Vec2(float x, float y);
    void set(float x, float y);
    float getX() const;
    float getY() const;
    ~Vec2();
};
