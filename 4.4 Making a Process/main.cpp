#include <Windows.h>
#include <stdio.h>

#define EXE_FILENAME "C:\\Users\\xbont\\source\\repos\\operating_systems_training\\4.4 Making a Process\\x64\\Hello_World.exe"
#define PROCESS_ARG "4"

int main()
{
	STARTUPINFOA si; //Default Startup info for CreateProcessA.
	PROCESS_INFORMATION pi; //Default Process info for CreateProcessA.

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// create argument string
	CHAR exe_filename[] = EXE_FILENAME;
	CHAR process_arg[] = PROCESS_ARG;
	INT size = strlen(exe_filename) + strlen(process_arg) + 2;
	PCHAR param = (PCHAR)malloc(size * sizeof(CHAR)); //Allocate memory for command line parameter for CreateFileA.
	sprintf_s(param, size, "%s %s", exe_filename, process_arg); //String command line parameter.
	
	// Start the child process.
	BOOL success_flag = TRUE;
	success_flag = CreateProcessA(NULL,
		param, // Command line
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
	free(param);

	if (!success_flag) {
		printf("False to Create the Process. Error code: %d\n", GetLastError());
	}

	return 0;
}