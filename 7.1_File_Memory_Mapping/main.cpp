/*
Info: Open a file and regex the amount of "A" chars appering in him, using mapping functions.
Author: Tomer
Date: 21/05/2023
*/

#include <Windows.h>
#include <stdio.h>

#define BUFFER_SIZE 65536 
#define CHAR_TO_SEARCH 'A'
#define FILENAME "gibrish.bin"

int main()
{
    HANDLE hFile = NULL;
    HANDLE hMapFile = NULL;
    CHAR inBuffer[BUFFER_SIZE];
    LPSTR pbuffer = inBuffer;
    DWORD nNumberOfBytesToRead = BUFFER_SIZE;
    DWORD nNumberOfBytesRead;

    CHAR input_file[12] = FILENAME;
    LPCSTR p_input_file = input_file;

    // Making a handle open an existing bin file (must be in same location with sourcecode main.cpp)
    hFile = CreateFileA(p_input_file,
        GENERIC_READ,
        0,              // Other processes can't share this handle.
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    DWORD file_size = GetFileSize(hFile, NULL); //Get File size.

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("ERROR: %u :unable to create file \"%s\".\n",
            GetLastError(),
            p_input_file);
    }

    hMapFile = CreateFileMappingA(hFile, 
        NULL,               /*not inherited*/
        PAGE_READONLY,     /*read write attributes*/
        0,                  /*high-order size*/
        0,                  /*low-order size*/
        "myFile");              /*file name for sharing*/

    if (NULL == hMapFile)
    {
        printf("ERROR:%u: Failed to create File Mapping.\n",
            GetLastError());
        return 1;
    }

    int char_counter = 0;

    printf("[DEBUG]: Start reading file.\n");
    while (TRUE) {

        pbuffer = (LPSTR)MapViewOfFile(hMapFile, // handle to map object
            FILE_MAP_READ,  // read permission
            0,
            0,
            BUFFER_SIZE);

        if (NULL == pbuffer)
        {
            printf("Could not map view of file, Or hand of file. Error: (%d).\n",
                GetLastError());

            break;
        }

        for (int i = 0; i < BUFFER_SIZE; i++) {
            if (inBuffer[i] == CHAR_TO_SEARCH) char_counter++;
        }

        UnmapViewOfFile(pbuffer);
    }
    
    printf("Work properly. The number of times 'A' shown in the file is: %d \n", char_counter);

    CloseHandle(hMapFile);
    CloseHandle(hFile);
    return 0;
}

int main();