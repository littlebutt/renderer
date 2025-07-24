#ifndef __CAMERA_H
#define __CAMERA_H

#include "matrix.h"
#include "vector.h"
#include "config.h"

matrix lookat(vector3 eye, vector3 center, vector3 up);

matrix viewport(int x, int y, int w, int h);

#endif // __CAMERA_H