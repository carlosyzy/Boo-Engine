#pragma once
#include <string>

class Color
{
private:
    float _r;
    float _g;
    float _b;
    float _a;
    std::string _hexString;

public:
    Color();
    Color(std::string color);
    Color(float r, float g, float b, float a);
    void set(float r, float g, float b, float a);
    void set(std::string color);
    void setR(float r);
    void setG(float g);
    void setB(float b);
    void setA(float a);
    std::string hexString(){
        return this->_hexString;
    }
    float getR();
    float getG();
    float getB();
    float getA();
    ~Color();
};
