#pragma once
#include <string>

class Color
{
private:
    float _r;
    float _g;
    float _b;
    float _a;
public:
    Color();
    Color(float r, float g, float b, float a);
    void set(float r, float g, float b, float a);
    void set(std::string color);
    float getR();
    float getG();
    float getB();
    float getA();
    ~Color();
};
