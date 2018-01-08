#include <stdio.h>
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
