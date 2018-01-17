#include <stdio.h>
//#include <linux/types.h>
#include <stdint.h>

int main(void)
{
	char test_8[] = {0x00, 0x10};
	uint16_t *test_16 = (uint16_t *)test_8;

	printf("0x%x 0x%x\n", *test_8, *(test_8+1));
	//printf("0x%x 0x%x\n", *test_16, (*test_16) & 0xff00);

	return 0;
}