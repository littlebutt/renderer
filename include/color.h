#ifndef __COLOR_H__
#define __COLOR_H__

typedef struct {
    float r;
    float g;
    float b;
    float a;
} color;

color color_new(float r, float g, float b, float a);
color color_add(color a, color b);
color color_substract(color a, color b);
color color_multiply(color a, float scalar);
color color_divide(color a, float scalar);
color color_lerp(color a, color b, float t);

#endif // __COLOR_H__