#include "windows.h"
#include "stdio.h"

int mult(int);
int max_num = 10;
int* buf = (int*)malloc(max_num * sizeof(int));

int main()
{
	for (int i = 0; i < max_num; i++) {
		buf[i] = mult(i);;
		printf("%d\n", buf[i]);
	}
	return 0;
}

int mult(int num)
{
	static int my_num = 5;
	int result = num * 2 + my_num;
	return result;
}
