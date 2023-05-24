/// Info: Open a file and regex the amount of "A" chars appering in him.
/// Author: Tomer
/// Date: 21/05/2023

#include <Windows.h>
#include <stdio.h>

#define BUFFER_SIZE 1000 
#define CHAR_TO_SEARCH 'A'
#define FILENAME "gibrish.bin"

int main()
{
    HANDLE hFile = NULL;
    CHAR inBuffer[BUFFER_SIZE];
    LPVOID pbuffer = inBuffer;
    DWORD nNumberOfBytesToRead = BUFFER_SIZE;
    DWORD nNumberOfBytesRead;

    CHAR input_file[12] = FILENAME;
    LPCSTR p_input_file = input_file;

    // Making a handle open an existing bin file (must be in same location with sourcecode main.cpp)
    hFile = CreateFileA(p_input_file,
        GENERIC_READ,
        FILE_SHARE_READ, // Other processes can share this handle.
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

    int char_counter = 0;

    printf("[DEBUG]: Start reading file.\n");
    while (TRUE) {
        BOOL reading_result = ReadFile(hFile,
            pbuffer,
            nNumberOfBytesToRead,
            &nNumberOfBytesRead,
            NULL);

        if ((int)nNumberOfBytesRead == 0) {
            printf("[DEBUG]: EOF \n");
            break;
        }

        if (!reading_result)
        {
            printf("Worked wrong. error code: %d. \n", GetLastError());
            return 1;
        }

        for (int i = 0; i < BUFFER_SIZE; i++) {
            if (inBuffer[i] == CHAR_TO_SEARCH) char_counter++;
        }
    }
    
    printf("Work properly. The number of times 'A' shown in the file is: %d \n", char_counter);

    CloseHandle(hFile);
    return 0;
}

int main();