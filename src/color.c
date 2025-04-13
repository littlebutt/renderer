#include "color.h"

color color_new(float r, float g, float b, float a)
{
    color c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

color color_add(color a, color b)
{
    color r;
    r.r = a.r + b.r;
    r.g = a.g + b.g;
    r.b = a.b + b.b;
    r.a = a.a + b.a;
    return r;
}

color color_substract(color a, color b)
{
    color r;
    r.r = a.r - b.r;
    r.g = a.g - b.g;
    r.b = a.b - b.b;
    r.a = a.a - b.a;
    return r;
}

color color_multiply(color a, float scalar)
{
    color r;
    r.r = a.r * scalar;
    r.g = a.g * scalar;
    r.b = a.b * scalar;
    r.a = a.a * scalar;
    return r;
}

color color_divide(color a, float scalar)
{
    color r;
    r.r = a.r / scalar;
    r.g = a.g / scalar;
    r.b = a.b / scalar;
    r.a = a.a / scalar;
    return r;
}

color color_lerp(color a, color b, float t)
{
    color r;
    r.r = a.r + (b.r - a.r) * t;
    r.g = a.g + (b.g - a.g) * t;
    r.b = a.b + (b.b - a.b) * t;
    r.a = a.a + (b.a - a.a) * t;
    return r;
}

color WHITE = {1.0f, 1.0f, 1.0f, 1.0f};