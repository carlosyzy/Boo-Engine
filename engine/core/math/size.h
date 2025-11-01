#pragma once
#include <string>
class Size
{
private:
    float _width;
    float _height;

public:
    Size();
    Size(float width, float height);
    void set(float width, float height);
    void setWidth(float width);
    void setHeight(float height);
    float getWidth() const;
    float getHeight() const;

    static Size Default;

    ~Size();
};
