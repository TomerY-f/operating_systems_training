/// Author - Tomer Yehezqel 18/05/2023
/// A process represents philosopher eating. 
/// The process checks that's no one using chopsticks mutexs before acting.
/// return: 1 as success.

#include <Windows.h>
#include <stdio.h>

#define LOOP_SIZE 20

int main(int argc, char* argv[]) {
	// Number of arguments validation:
	if (argc != 2) {
		printf("Incorrect number of arguments\n");
		return 0;
	}
	printf("DEBUG: Process %d is running.\n", atoi(argv[1]));

	// Variables decleration
	INT eating_counter = 0;

	//Initialize the common mutexs - chopsticks:
	HANDLE hChopsticksMutex[5];
	INT number_of_chopsticks = sizeof(hChopsticksMutex) / sizeof(hChopsticksMutex[0]);

	for (INT i = 0; i < number_of_chopsticks; i++) {
		CHAR chopstick_name[20] = "chopstick_mutex";
		CHAR index[] = { i + '0', 0 };
		strcat_s(chopstick_name, index);
		hChopsticksMutex[i] = CreateMutexA(NULL,
			FALSE,
			chopstick_name);
		//printf("DEBUG: Mutex name: %s initiated on Process %d\n", chopstick_name, atoi(argv[1]));
	}

	//Mutex occupation debugging:
	//printf("\nMutex available = %d\n", WAIT_OBJECT_0);
	//printf("Mutex 0: %d\n", WaitForSingleObject(hChopsticksMutex[0], 0));
	//printf("Mutex 1: %d\n", WaitForSingleObject(hChopsticksMutex[1], 0));
	//printf("Mutex 2: %d\n", WaitForSingleObject(hChopsticksMutex[2], 0));
	//printf("Mutex 3: %d\n", WaitForSingleObject(hChopsticksMutex[3], 0));
	//printf("Mutex 4: %d\n\n", WaitForSingleObject(hChopsticksMutex[4], 0));

	while (eating_counter < LOOP_SIZE) {
		//printf("DEBUG: Mutex name: % dEnter  the action loop\n", atoi(argv[1]));
		//Check if two mutexs are not occupied, if not, act:
		for (INT i = 0; i < number_of_chopsticks; i++) {
			DWORD first_chopstick_availability_Result = WaitForSingleObject(hChopsticksMutex[i],
				0);
			if (first_chopstick_availability_Result == WAIT_OBJECT_0) {
				for (INT j = 0; j < number_of_chopsticks; j++) {
					DWORD second_chopstick_availability_Result = WaitForSingleObject(hChopsticksMutex[j],
						0);
					if (second_chopstick_availability_Result == WAIT_OBJECT_0) {
						printf("Philosopher %d eat %d times.\n", atoi(argv[1]), eating_counter);
						eating_counter++;
						Sleep(500);

						// Release each eating session one chopstick.
						ReleaseMutex(hChopsticksMutex[i]);
						break;
					}
					else
					{
						//printf("DEBUG: Mutex %d is not available.\n", j);
						continue;
					}
				}
			}
			else
			{
				//printf("DEBUG: Mutex %d is not available.\n", i);
				continue;
			}
			break;
		}
	}

	// Close all Handles
	for (INT i = 0; i < number_of_chopsticks; i++) {
		CloseHandle(hChopsticksMutex[i]);
	}
	printf("DEBUG: Process %d is exit.\n", atoi(argv[1]));
	return 1;
}