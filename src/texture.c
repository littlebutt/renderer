#include "texture.h"
#include <math.h>

static float _texture_clamp(float min, float max, float value)
{
	if (value > max)
	{
        return max;
    }
    else if (value < min)
    {
        return min;
    }
    else
    {
        return value;
    }
}

color texture_sample(texture* tex, float u, float v)
{
    float _u = _texture_clamp(0.0f, 1.0f, u);
    float _v = _texture_clamp(0.0f, 1.0f, v);
    int width = fmin((int)(_u * tex->width), tex->width - 1);
    int height = fmin((int)((_v) * tex->height), tex->height - 1);
    return tex->data[tex->width * height + width];
}