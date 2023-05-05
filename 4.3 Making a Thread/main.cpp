#include <windows.h>
#include <stdio.h>


//Convention for function define in WinApi:
// < DWORD WINAPI function_name(LPVOID lparam); > 
//Can call only for one parameter.

struct Variables
{
	INT a;
	INT b;
};

DWORD WINAPI sum(LPVOID lparam) {

	struct Variables* nums = (Variables*)lparam;
	INT sum = nums->a + nums->b;
	printf("The sum is: %d\n", sum);

	Sleep(100000); // Suspension of code execution.
	return 1;

}

int main()
{
	struct Variables* my_struct =
		(Variables*)malloc(sizeof(Variables));

	my_struct->a = 3;
	my_struct->b = 9;

	LPVOID pstruct = my_struct;
	
	HANDLE hThread;
	

	// Create the thread to begin execution on its own.
	hThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		sum,					// thread function name
		pstruct,				// argument to thread function 
		0,                      // use default creation flags 
		NULL);					// returns the thread identifier 

	WaitForSingleObject(hThread, INFINITE);


	CloseHandle(hThread);
}


