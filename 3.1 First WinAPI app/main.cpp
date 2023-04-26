#include <windows.h>
#include <stdio.h>

WCHAR input_file[9] = L"test.txt";
LPCWSTR p_input_file = input_file;

#define BUFFER_SIZE 20

int main()
{
    HANDLE hFile;
    CHAR inBuffer[BUFFER_SIZE];
    LPVOID pbuffer = inBuffer;
    DWORD nNumberOfBytesToRead = BUFFER_SIZE;

    hFile = CreateFile(input_file,               // file to open
        GENERIC_READ,          // open for reading
        FILE_SHARE_READ,       // share for reading
        NULL,                  // default security
        OPEN_EXISTING,         // existing file only
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
        NULL);                 // no attr. template

    BOOL reading_result = ReadFile(hFile,
        pbuffer,
        nNumberOfBytesToRead,
        NULL,
        NULL);

    if (reading_result == TRUE)
    {
        printf("Work properly. The text is: %s \n", inBuffer);
    }
    else
    {
        printf("Work wrong. error code: %d. \n", GetLastError());
    }
    

    CloseHandle(hFile);
}

