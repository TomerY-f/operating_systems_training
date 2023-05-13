#include <Windows.h>
#include <stdio.h>

#define EXE_FILENAME "C:\\Users\\xbont\\source\\repos\\operating_systems_training\\5.2 Mutex_mid_exercise\\HelloWorldMutex.exe"
#define FIRST_PROCESS_ARG "14"
#define SECOND_PROCESS_ARG "69"

int main()
{
	STARTUPINFOA si; //Default Startup info for CreateProcessA.
	PROCESS_INFORMATION pi; //Default Process info for CreateProcessA.

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// create argument string for processes:
	CHAR exe_filename[] = EXE_FILENAME;
	// First process:
	CHAR first_process_arg[] = FIRST_PROCESS_ARG;
	INT first_process_size = strlen(exe_filename) + strlen(first_process_arg) + 2;
	PCHAR first_process_param = (PCHAR)malloc(first_process_size * sizeof(CHAR)); //Allocate memory for command line parameter for CreateFileA.
	sprintf_s(first_process_param, first_process_size, "%s %s", exe_filename, first_process_arg); //String command line parameter.

	// Second process:
	CHAR second_process_arg[] = SECOND_PROCESS_ARG;
	INT second_process_size = strlen(exe_filename) + strlen(second_process_arg) + 2;
	PCHAR second_process_param = (PCHAR)malloc(second_process_size * sizeof(CHAR)); //Allocate memory for command line parameter for CreateFileA.
	sprintf_s(second_process_param, second_process_size, "%s %s", exe_filename, second_process_arg); //String command line parameter.

	// Creating mutex to synchronize processes:
	HANDLE hHelloWorldMutex = CreateMutexA(NULL,
		FALSE,
		"hello_world_mutex");

	// Start the child processes.
	BOOL first_success_flag = TRUE;
	first_success_flag = CreateProcessA(NULL,
		first_process_param, // Command line
		NULL, // Process handle not inheritable
		NULL, // Thread handle not inheritable
		FALSE, // Set handle inheritance to FALSE
		0, // No creation flags
		NULL, // Use parent's environment block
		NULL, // Use parent's starting directory
		&si, // Pointer to STARTUPINFO structure
		&pi); // Pointer to PROCESS_INFORMATION structure

	BOOL second_success_flag = TRUE;
	second_success_flag = CreateProcessA(NULL,
		second_process_param, // Command line
		NULL, // Process handle not inheritable
		NULL, // Thread handle not inheritable
		FALSE, // Set handle inheritance to FALSE
		0, // No creation flags
		NULL, // Use parent's environment block
		NULL, // Use parent's starting directory
		&si, // Pointer to STARTUPINFO structure
		&pi); // Pointer to PROCESS_INFORMATION structure

	// Close Handels and free allocation
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	ReleaseMutex(hHelloWorldMutex);
	CloseHandle(hHelloWorldMutex);
	free(first_process_param);
	free(second_process_param);

	if (!first_success_flag) {
		printf("False to Create the First Process. Error code: %d\n", GetLastError());
	}
	if (!second_success_flag) {
		printf("False to Create the Second Process. Error code: %d\n", GetLastError());
	}

	return 0;
}