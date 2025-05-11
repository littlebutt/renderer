#include "vector.h"

vector2 vector2_new(float x, float y)
{
    vector2 v;
    v.x = x;
    v.y = y;
    return v;
}

vector2 vector2_add(vector2 a, vector2 b)
{
    vector2 v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    return v;
}

vector2 vector2_subtract(vector2 a, vector2 b)
{
    vector2 v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    return v;
}

vector2 vector2_multiply(vector2 v, float value)
{
    vector2 r;
    r.x = v.x * value;
    r.y = v.y * value;
    return r;
}

vector2 vector2_divide(vector2 v, float value)
{
    vector2 r;
    r.x = v.x / value;
    r.y = v.y / value;
    return r;
}

vector3 vector3_new(float x, float y, float z)
{
    vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vector3 vector3_add(vector3 a, vector3 b)
{
    vector3 v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    return v;
}

vector3 vector3_subtract(vector3 a, vector3 b)
{
    vector3 v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    return v;
}

vector3 vector3_multiply(vector3 a, float value)
{
    vector3 r;
    r.x = a.x * value;
    r.y = a.y * value;
    r.z = a.z * value;
    return r;
}

vector3 vector3_divide(vector3 a, float value)
{
    vector3 r;
    r.x = a.x / value;
    r.y = a.y / value;
    r.z = a.z / value;
    return r;
}

float vector3_dot(vector3 a, vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vector3 vector3_cross(vector3 a, vector3 b)
{
    vector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

vector3 vector3_lerp(vector3 a, vector3 b, float t)
{
    return vector3_add(a, vector3_multiply(vector3_subtract(b, a), t));
}

vector3 vector3_normalize(vector3 a)
{
    float sm = vector3_dot(a, a); 
    float ssm = sqrt(sm);
    return vector3_divide(a, ssm);
}