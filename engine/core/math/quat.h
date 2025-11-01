#pragma once
class Quat
{
private:
    float _x;
    float _y;
    float _z;
    float _w;

public:
    Quat();
    Quat(float x, float y, float z, float w);
    void set(float x, float y, float z, float w);
    float getX();
    float getY();
    float getZ();
    float getW();
    ~Quat();
};
