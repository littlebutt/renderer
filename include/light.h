#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "vector.h"

typedef struct
{
    float intensity;
    vector3 position;
    vector3 direction;
} parallel_light;

parallel_light parallel_light_new(float intensity, vector3 position, vector3 direction);

vector3 parallel_light_direction(parallel_light pl);

matrix4 parallel_light_look_at(parallel_light pl, vector3 up_axis);

#endif // __LIGHT_H__