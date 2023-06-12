#include "../inc/wlc_bn.h"


void bn_print(const dig_t* x, int digs)
{
	int i = digs - 1;//digs - 存储大整数所用的数组的最后一个元素x[digs - 1]

	printf("\n0x");
	for (; i >= 0; i--)
	{
		printf("%08X", x[i]);
	}
	printf("\n");
}