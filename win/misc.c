#include "misc.h"

#include <stdio.h>

size_t getline(char **linep, size_t len, FILE *fp)
{
    *linep = (char *)malloc(len);
    if (*linep == NULL)
    {
        return 0;
    }

    return fgets(*linep, len, fp) != NULL;
}