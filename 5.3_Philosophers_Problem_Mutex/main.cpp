#include <Windows.h>
#include <stdio.h>

#define EXE_FILENAME "C:\\Users\\xbont\\source\\repos\\operating_systems_training\\5.3_Philosophers_Problem_Mutex\\Eating_Philosopher.exe"

int main()
{
	STARTUPINFOA si; //Default Startup info for CreateProcessA.
	PROCESS_INFORMATION pi; //Default Process info for CreateProcessA.

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// create argument string for processes:
	CHAR exe_filename[] = EXE_FILENAME;

	CHAR process_args[5];
	PCHAR processes_param[5];
	INT number_of_processes = sizeof(process_args) / sizeof(process_args[0]);


	// Init processes command line:
	for (INT i = 0; i < number_of_processes; i++) {
		_itoa_s(i, process_args, 10);
		INT process_size = strlen(exe_filename) + strlen(process_args) + 2;
		processes_param[i] = (PCHAR)malloc(process_size * sizeof(CHAR)); //Allocate memory for command line parameter for CreateFileA.
		sprintf_s(processes_param[i], process_size, "%s %s", exe_filename, process_args); //String command line parameter.
	}

	// Creating mutex to synchronize processes:
	HANDLE hChopsticksMutex[5];
	INT number_of_chopsticks = sizeof(hChopsticksMutex) / sizeof(hChopsticksMutex[0]);

	for (INT i = 0; i < number_of_chopsticks; i++) {

		CHAR chopstick_name[20] = "chopstick_mutex";
		CHAR index[] = { i + '0', 0 };
		strcat_s(chopstick_name, index);
		hChopsticksMutex[i] = CreateMutexA(NULL,
			FALSE,
			chopstick_name);
	}

	BOOL success_flags[5];
	INT number_of_success_flags = number_of_processes;

	// Execute the child processes.
	for (INT i = 0; i < number_of_success_flags; i++) {
		success_flags[i] = TRUE;
		success_flags[i] = CreateProcessA(NULL,
			processes_param[i], // Command line
			NULL, // Process handle not inheritable
			NULL, // Thread handle not inheritable
			FALSE, // Set handle inheritance to FALSE
			0, // No creation flags
			NULL, // Use parent's environment block
			NULL, // Use parent's starting directory
			&si, // Pointer to STARTUPINFO structure
			&pi); // Pointer to PROCESS_INFORMATION structure
	}

	// Close Handels and free allocation
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	for (INT i = 0; i < number_of_chopsticks; i++) {
		ReleaseMutex(hChopsticksMutex[i]);
		CloseHandle(hChopsticksMutex[i]);
		free(processes_param[i]);
	}

	// Error handling
	for (INT i = 0; i < number_of_success_flags; i++) {
		if (!success_flags[i]) {
			printf("False to Create the %d Process. Error code: %d\n", success_flags[i], GetLastError());
		}
	}

	return 1;
}