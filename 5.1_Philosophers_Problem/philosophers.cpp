#include <windows.h>
#include <stdio.h>

#define LOOP_SIZE 50

CRITICAL_SECTION chopstick01;
CRITICAL_SECTION chopstick02;
CRITICAL_SECTION chopstick03;
CRITICAL_SECTION chopstick04;
CRITICAL_SECTION chopstick05;

struct philosopher_identifier
{
	INT counter;
	CHAR name[20];
};

// Threads function - ping and count.
DWORD WINAPI philosopher_eating(LPVOID lparam) {

	EnterCriticalSection(&chopstick01);

	struct philosopher_identifier* thread = (philosopher_identifier*)lparam;

	while (thread->counter < LOOP_SIZE)
	{
		printf("%s eat %d times.\n", thread->name, thread->counter);
		thread->counter = thread->counter + 1;
		Sleep(1000);
	}
	LeaveCriticalSection(&chopstick01);
	return 1;
}

int main()
{
	// Locks initializing
	InitializeCriticalSection(&chopstick01);
	InitializeCriticalSection(&chopstick02);
	InitializeCriticalSection(&chopstick03);
	InitializeCriticalSection(&chopstick04);
	InitializeCriticalSection(&chopstick05);
	
	// Threads decleration
	struct philosopher_identifier* threads[5];
	LPVOID pthreads[5]; 
	HANDLE hPhilosophers[5];


	// Create the threads to begin execution on its own.
	for (INT i = 0; i < sizeof(hPhilosophers); i++) {
		
		threads[i] = (philosopher_identifier*)malloc(sizeof(philosopher_identifier));
		threads[i]->counter = 0;
		
		CHAR name[20] = "Philosopher "; ////check this
		CHAR index[] = { i + '0' };
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
	WaitForMultipleObjects(sizeof(hPhilosophers), hPhilosophers, TRUE, INFINITE);
	for (INT i = 0; i < sizeof(hPhilosophers); i++) {
		CloseHandle(hPhilosophers[i]);
	}
	

	// Lock deletion
	DeleteCriticalSection(&chopstick01);
	DeleteCriticalSection(&chopstick02);
	DeleteCriticalSection(&chopstick03);
	DeleteCriticalSection(&chopstick04);
	DeleteCriticalSection(&chopstick05);
}

