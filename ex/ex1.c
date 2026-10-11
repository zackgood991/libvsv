/* simple VSV example - 42 * 2^i */

#include <stdio.h>
#include "vsv.h"

int main(int argc, char** argv)
{
	int i;
	int topush = 42;
	int* gotten;
	VSV vec;

	vsv_init(sizeof(topush), &vec);

	for(i = 0; i < 10; i++)
	{
		vsv_push_back(&topush, &vec);
		if(i == 0)
			printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));
		topush *= 2;
	}
	vsv_insert_n(10, 3, &topush, &vec);

loop:
	for(i = 0; (gotten = (int*)vsv_at(i, &vec)) != NULL; i++)
	{
		printf("vec[%4d/%lu]: %d\n", i, vsv_size(&vec), *gotten);
	}
	vsv_pop_back(&vec);
	if(vsv_size(&vec) > 0)
		goto loop;


	printf("...\n");

	printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));
	vsv_resize_from(56, &topush, &vec);
	printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));
	for(i = 0; (gotten = (int*)vsv_at(i, &vec)) != NULL; i++)
	{
		printf("vec[%4d/%lu]: %d\n", i, vsv_size(&vec), *gotten);
		if(i >= 10) *gotten += i;
	}
	vsv_resize(58, &vec);
	printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));
	for(i = 0; (gotten = (int*)vsv_at(i, &vec)) != NULL; i++)
	{
		printf("vec[%4d/%lu]: %d\n", i, vsv_size(&vec), *gotten);
	}
	printf("%d\n", vsv_erase_n(55, 2, &vec));
	printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));
	for(i = 0; (gotten = (int*)vsv_at(i, &vec)) != NULL; i++)
	{
		printf("vec[%4d/%lu]: %d\n", i, vsv_size(&vec), *gotten);
	}
	vsv_clear(&vec);
	printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));
	vsv_push_back(&topush, &vec);
	printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));
	vsv_shrink_to_fit(&vec);
	printf("size: %lu, cap: %lu\n", vsv_size(&vec), vsv_capacity(&vec));

	return 0;
}
