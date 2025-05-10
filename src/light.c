#include "light.h"

parallel_light parallel_light_new(float intensity, vector3 position, vector3 direction)
{
    parallel_light pl = {intensity, position, direction};
    return pl;
}

vector3 parallel_light_direction(parallel_light pl)
{
    return vector3_subtract(vector3_new(0.0f, 0.0f, 0.0f, 0.0f), pl.direction);
}

matrix4 parallel_light_look_at(parallel_light pl, vector3 up_axis)
{
    vector3 look_dir = pl.direction;
    look_dir = vector3_normalize(look_dir);
    vector3 right_dir = vector3_cross(up_axis, look_dir);
    right_dir = vector3_normalize(right_dir);
    vector3 up_dir = vector3_cross(look_dir, right_dir);
    up_dir = vector3_normalize(up_dir);

    matrix4 r;
    r.value[0][0] = right_dir.x;
    r.value[1][0] = up_dir.x;
    r.value[2][0] = look_dir.x;
    r.value[3][0] = 0.0f;
    r.value[0][1] = right_dir.y;
    r.value[1][1] = up_dir.y;
    r.value[2][1] = look_dir.y;
    r.value[3][1] = 0.0f;
    r.value[0][2] = right_dir.z;
    r.value[1][2] = up_dir.z;
    r.value[2][2] = look_dir.z;
    r.value[3][2] = 0.0f;
    r.value[0][3] = -pl.position.x;
    r.value[1][3] = -pl.position.y;
    r.value[2][3] = -pl.position.z;
    r.value[3][3] = 1.0f;
    return r;
}