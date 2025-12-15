
#include "color.h"
#include <iostream>
#include <regex> // 主要头文件

Color::Color()
{
    this->set(1.0f, 1.0f, 1.0f, 1.0f);
}
Color::Color(std::string color)
{
    this->set(color);
}
Color::Color(float r, float g, float b, float a)
{
    this->set(r, g, b, a);
}
void Color::set(float r, float g, float b, float a)
{
    this->_r = r;
    this->_g = g;
    this->_b = b;
    this->_a = a;
}
void Color::set(std::string color)
{
    // 支持格式: #RRGGBB 或 #RRGGBBAA
    static const std::regex colorRegex("^#[0-9A-Fa-f]{6}([0-9A-Fa-f]{2})?$");

    if (!std::regex_match(color, colorRegex))
    {
        std::cerr << "Color::set: invalid color format: " << color << std::endl;
        return;
    }

    try
    {
        this->_r = std::stoi(color.substr(1, 2), nullptr, 16) / 255.0f;
        this->_g = std::stoi(color.substr(3, 2), nullptr, 16) / 255.0f;
        this->_b = std::stoi(color.substr(5, 2), nullptr, 16) / 255.0f;

        // 如果有Alpha分量
        if (color.size() == 9)
        {
            this->_a = std::stoi(color.substr(7, 2), nullptr, 16) / 255.0f;
        }
        else
        {
            this->_a = 1.0f;
        }
        this->_hexString = color;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Color::set: conversion error: " << e.what() << std::endl;
    }
}
void Color::setR(float r)
{
    this->_r = r;
}
void Color::setG(float g)
{
    this->_g = g;
}
void Color::setB(float b)
{
    this->_b = b;
}
void Color::setA(float a)
{
    this->_a = a;
}
float Color::getR() const
{
    return this->_r;
}

float Color::getG() const
{
    return this->_g;
}

float Color::getB() const
{
    return this->_b;
}

float Color::getA() const
{
    return this->_a;
}
Color::~Color()
{
}
