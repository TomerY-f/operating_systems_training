#include <windows.h>
#include <stdio.h>

//Convention for function define in WinApi:
// < DWORD WINAPI function_name(LPVOID lparam); > 
//Can call only for one parameter.
#define LOOP_SIZE 1000

struct Thread_Identifier
{
	INT index;
	CHAR name[10];
};

// Threads function - ping and count.
DWORD WINAPI ping_and_count(LPVOID lparam) {

	struct Thread_Identifier* thread = (Thread_Identifier*)lparam;

	while (thread->index < LOOP_SIZE)
	{
		printf("%s count is: %d\n", thread->name, thread->index);
		thread->index = thread->index +1;
		Sleep(1000);
	}
	return 1;
}

int main()
{

	// Threads decleration
	struct Thread_Identifier* thread_1 = (Thread_Identifier*)malloc(sizeof(Thread_Identifier));
	thread_1->index = 0;
	strcpy(thread_1->name, "thread_1");

	struct Thread_Identifier* thread_2 = (Thread_Identifier*)malloc(sizeof(Thread_Identifier));
	thread_2->index = 0;
	strcpy(thread_2->name, "thread_2");

	struct Thread_Identifier* thread_3 = (Thread_Identifier*)malloc(sizeof(Thread_Identifier));
	thread_3->index = 0;
	strcpy(thread_3->name, "thread_3");

	struct Thread_Identifier* thread_4 = (Thread_Identifier*)malloc(sizeof(Thread_Identifier));
	thread_4->index = 0;
	strcpy(thread_4->name, "thread_4");

	LPVOID pthread_1 = thread_1;
	LPVOID pthread_2 = thread_2;
	LPVOID pthread_3 = thread_3;
	LPVOID pthread_4 = thread_4;
	LPVOID pthreads[4] = {pthread_1, pthread_2, pthread_3, pthread_4};
	HANDLE hThreadHandles[4];
	

	// Create the threads to begin execution on its own.
	for (INT i = 0; i < 4; i++) {
		hThreadHandles[i] = CreateThread(
			NULL,
			0,
			ping_and_count,
			pthreads[i],
			0,
			NULL);
	}

	//Wait for threads to finish and close them.
	if ((hThreadHandles[0] != NULL) && (hThreadHandles[1] != NULL) && (hThreadHandles[2] != NULL) && (hThreadHandles[3] != NULL)) {
		WaitForMultipleObjects(4, hThreadHandles, TRUE, INFINITE);
		for (INT i = 0; i < 4; i++) {
			CloseHandle(hThreadHandles[i]);
		}
	}
}


