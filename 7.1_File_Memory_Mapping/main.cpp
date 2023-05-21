/// Info: Open a file and regex the amount of "A" chars appering in him.
/// Author: Tomer
/// Date: 21/05/2023

#include <Windows.h>
#include <stdio.h>

WCHAR input_file[12] = L"gibrish.bin";
LPCWSTR p_input_file = input_file;

#define BUFFER_SIZE 1000

int main()
{
    HANDLE hFile = NULL;
    CHAR inBuffer[BUFFER_SIZE];
    LPVOID pbuffer = inBuffer;
    DWORD nNumberOfBytesToRead = BUFFER_SIZE;
    DWORD nNumberOfBytesRead;

    // Making a handle open an existing bin file (must be in same location with sourcecode main.cpp)
    hFile = CreateFile(input_file,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    int error_handle_code = GetLastError();
    if (error_handle_code != 0)
    {
        printf("Handle is loaded in invalid way. Error code: %d\n", error_handle_code);
        return 1;
    }

    // Using the handle to ReadData from the file into a buffer.
    // Important - The buffer reads bytes as the nNumberOfBytesToRead defines him, without null-terminator to the string inside the text file.
    BOOL reading_result = ReadFile(hFile,
        pbuffer,
        nNumberOfBytesToRead,
        &nNumberOfBytesRead,
        NULL);

    if (!reading_result)
    {
        printf("Worked wrong. error code: %d. \n", GetLastError());
        return 1;
    }

    printf("Work properly. The text is: %.*s \n", (int)nNumberOfBytesRead, inBuffer);


    CloseHandle(hFile);
    return 0;
}

int main();