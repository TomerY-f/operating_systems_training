#include <Windows.h>
#include <stdio.h>
#include <iostream>

// argc - argument count - means number of arguments the function recevies. 
// aegv - argument vector - an array of pointers to arguments.

// In the example below, it can be shown in debugging that argc will include two arguments for typing one input for the exe - one will be the value of the argument, 
//																															  and the other will be the address of the argument.


int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Incorrect number of arguments\n");
		return 0;
	}
	int id = atoi(argv[1]);
	printf("Hello World! Process got argument %d\n", id);
	return 1;
}
