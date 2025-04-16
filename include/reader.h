#ifndef __READER_H__
#define __READER_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "texture.h"

mesh *read_obj(char *filename);

texture *read_bmp(char *filename);

#endif // __READER_H__