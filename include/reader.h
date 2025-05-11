#ifndef __READER_H__
#define __READER_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "vertex.h"
#include "model.h"

model *read_obj(char *filename);

texture *read_bmp(char *filename);

texture *read_tga(char *filename);

#endif // __READER_H__