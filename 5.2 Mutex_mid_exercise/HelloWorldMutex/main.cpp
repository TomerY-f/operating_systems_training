/// Author - Tomer Yehezqel 13/05/2023 
/// A process prints the arguments he gets from the command line.
/// The process check that's no one uses hello_world_mutex before acting. 
/// return: 1 succsess
#include <Windows.h>
#include <stdio.h>
#include <iostream>

#define FIVE_SECONDS 5000

int main(int argc, char *argv[]) {
	//Initialize the common mutex:
	HANDLE hHelloWorldMutex = CreateMutexA(NULL,
		FALSE,
		"hello_world_mutex");
	
	//Check if the mutex is occupied:
	DWORD waitResult = WaitForSingleObject(hHelloWorldMutex,
		INFINITE);

	if (waitResult == WAIT_OBJECT_0) {
		printf("Hello World! Process got argument %d.\n", atoi(argv[1]));
		Sleep(FIVE_SECONDS);
		ReleaseMutex(hHelloWorldMutex);
		CloseHandle(hHelloWorldMutex);
		return 1;
	}
}