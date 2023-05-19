#include <Windows.h>
#include <stdio.h>

#define EXE_FILENAME "C:\\Users\\xbont\\source\\repos\\operating_systems_training\\5.3_Philosophers_Problem_Mutex\\Eating_Philosopher\\x64\\Debug\\Eating_Philosopher.exe"

int main()
{
	// create argument string for processes:
	CHAR exe_filename[] = EXE_FILENAME;

	CHAR process_args[5];
	PCHAR processes_param[5];
	INT number_of_processes = sizeof(process_args) / sizeof(process_args[0]);

	//Processes info:
	STARTUPINFOA si; //Default Startup info for CreateProcessA.
	PROCESS_INFORMATION pi[5]; //Default Process info for CreateProcessA. Must be a constant value - that's why usnig magic number 5 and not number of processes.

	// Init processes command line:
	for (INT i = 0; i < number_of_processes; i++) {
		_itoa_s(i, process_args, 10);
		INT process_size = strlen(exe_filename) + strlen(process_args) + 3;
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
		printf("Mutex name: %s", chopstick_name);
	}

	BOOL success_flags[5];
	INT number_of_success_flags = number_of_processes;

	// Execute the child processes.
	for (INT i = 0; i < number_of_success_flags; i++) {
		// Startup and process info:
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi[i], sizeof(pi[i]));

		// Process creation
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
			&pi[i]); // Pointer to PROCESS_INFORMATION structure

	}

	// Wait until child processes exit:
	for (int i = 0; i < number_of_processes; i++) {
		WaitForSingleObject(pi[i].hProcess, INFINITE);
	}

	// Close Handels and free allocation
	for (INT i = 0; i < number_of_chopsticks; i++) {
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
		ReleaseMutex(hChopsticksMutex[i]);
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