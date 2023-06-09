#include <windows.h>
#include <stdio.h>
#include <ctime>

#define LOOP_SIZE 1000

CRITICAL_SECTION chopsticks[5];

struct philosopher_identifier
{
	INT counter;
	CHAR name[20];
};

// Threads function - Philosopher eats.
DWORD WINAPI philosopher_eating(LPVOID lparam) {

	INT number_of_chopsticks = sizeof(chopsticks) / sizeof(chopsticks[0]);
	for (INT i = 0; i < number_of_chopsticks; i++) {
		if (TryEnterCriticalSection(&chopsticks[i])) {
			for (INT j = 0; j < number_of_chopsticks; j++) {
				if (TryEnterCriticalSection(&chopsticks[i])) {
					struct philosopher_identifier* thread = (philosopher_identifier*)lparam;

					while (thread->counter < LOOP_SIZE)
					{
						printf("%s eat %d times.\n", thread->name, thread->counter);
						thread->counter = thread->counter + 1;
					}
				}
			}
		}
		LeaveCriticalSection(&chopsticks[i]);
	}

	return 1;
}

int main()
{	
	// Locks initializing
	INT number_of_chopsticks = sizeof(chopsticks) / sizeof(chopsticks[0]);
	for (INT i = 0; i < number_of_chopsticks; i++) {
		InitializeCriticalSection(&chopsticks[i]);
	}

	// Time objects
	time_t start_time;
	time_t end_time;
		
	// Threads decleration
	struct philosopher_identifier* threads[5];
	LPVOID pthreads[5]; 
	HANDLE hPhilosophers[5];

	// Create the threads to begin execution on its own.
	INT number_of_philosophers = sizeof(hPhilosophers) / sizeof(hPhilosophers[0]);
	time(&start_time);
	for (INT i = 0; i < number_of_philosophers; i++) {
		
		threads[i] = (philosopher_identifier*)malloc(sizeof(philosopher_identifier));
		threads[i]->counter = 0;
		
		CHAR name[20] = "Philosopher "; ////check this
		
		CHAR index[] = {i + '0', 0};
		strcat_s(name, index);
		strcpy_s(threads[i]->name, name);

		pthreads[i] = threads[i];

		hPhilosophers[i] = CreateThread(
			NULL,
			0,
			philosopher_eating,
			pthreads[i],
			0,
			NULL);
	}

	//Wait for threads to finish and close them.
	WaitForMultipleObjects(number_of_philosophers, hPhilosophers, TRUE, INFINITE);
	for (INT i = 0; i < number_of_philosophers; i++) {
		CloseHandle(hPhilosophers[i]);
	}

	// Lock deletion
	for (INT i = 0; i < number_of_chopsticks; i++) {
		DeleteCriticalSection(&chopsticks[i]);
	}

	// Running time measurement.
	time(&end_time);
	printf("The time took to all Philosophers eat %d times is: %d seconds.\n", LOOP_SIZE, (end_time - start_time));
	return 1;
}

