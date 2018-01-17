#include <stdio.h>
#include <stdlib.h> //malloc(), free(),
#include <string.h> //memset(),
#include "common.h"

void hex_dump(char *data, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		if (!(i % LINE_DIS_NUM))
			printf("\n");
		printf("0x%02x ", *(data+i) & 0xff);
	}

	printf("\n");
}

void *zelloc(size_t size)
{
    void *p = NULL;

    if (NULL == (p = malloc(size)))
        return NULL;

    memset(p, 0, size);

    return p;
}

void free_mem(void *p)
{
    if (NULL != p)
    {
        free(p);
        p = NULL;
    }
}

