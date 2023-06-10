#include <windows.h>
#include <stdio.h>

WCHAR input_file[9] = L"test.txt";
LPCWSTR p_input_file = input_file;

#define BUFFER_SIZE 30

void hooked_message_box() {
    int msgboxID = MessageBoxA(NULL,
        (LPCSTR)"Hacked by Tomer Yehezqel\n",
        (LPCSTR)"Hooked the function succeed",
        MB_DEFBUTTON2);
}

int hook(PCSTR func_to_hook, PCSTR DLL_to_hook, DWORD new_func_address);

int main()
{
    // File essentials: 
    HANDLE hFile = NULL;
    CHAR inBuffer[BUFFER_SIZE];
    LPVOID pbuffer = inBuffer;
    DWORD nNumberOfBytesToRead = BUFFER_SIZE;
    DWORD nNumberOfBytesRead;

    //Hook essentials:
    PCSTR func_to_hook = "CreateFileW";
    PCSTR DLL_to_hook = "KERNEL32.dll";
    DWORD new_func_address = (DWORD)&hooked_message_box;

    hook(func_to_hook, DLL_to_hook, new_func_address);
 
    // Making a handle open an existing txt file (must be in same location with sourcecode main.cpp)
    hFile = CreateFileW(input_file,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("Handle is loaded in invalid way. Error code: %d\n", GetLastError());
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

int hook(PCSTR func_to_hook, PCSTR DLL_to_hook, DWORD new_func_address) {
	// WORKS ONLY ON 32BIT Compileing 
	PIMAGE_DOS_HEADER dosHeader;
	PIMAGE_NT_HEADERS NTHeader;
	PIMAGE_OPTIONAL_HEADER32 optionalHeader; //Was 32 in barak code, Tomer.
	IMAGE_DATA_DIRECTORY importDirectory;
	DWORD descriptorStartRVA;
	PIMAGE_IMPORT_DESCRIPTOR importDescriptor;
	int index;

	// Get base address of currently running .exe
	DWORD baseAddress = (DWORD)GetModuleHandle(NULL);

	// Get the import directory address
	dosHeader = (PIMAGE_DOS_HEADER)(baseAddress); //dosHeader now is a struct with PE format headers

	if (((*dosHeader).e_magic) != IMAGE_DOS_SIGNATURE) {
		return 0;
	}

	// Locate NT header
	NTHeader = (PIMAGE_NT_HEADERS)(baseAddress + (*dosHeader).e_lfanew);
	if (((*NTHeader).Signature) != IMAGE_NT_SIGNATURE) {
		return 0;
	}

	// Locate optional header
	optionalHeader = &(*NTHeader).OptionalHeader ;
	if (((*optionalHeader).Magic) != 0x10B) { //64bit
		return 0;
	}

	importDirectory = (*optionalHeader).DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	descriptorStartRVA = importDirectory.VirtualAddress; 
	importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(descriptorStartRVA + baseAddress); 

	index = 0;
	char* DLL_name;
	// Look for the DLL which includes the function for hooking
	while (importDescriptor[index].Characteristics != 0) {
		DLL_name = (char*)(baseAddress + importDescriptor[index].Name);
		printf("DLL name: %s\n", DLL_name);
		if (!strcmp(DLL_to_hook, DLL_name))
			break;
		index++;
	}

	// exit if the DLL is not found in import directory
	if (importDescriptor[index].Characteristics == 0) { ///////// Tomer
		printf("DLL was not found");
		return 0;
	}

	// Search for requested function in the DLL
	PIMAGE_THUNK_DATA thunkILT; // Import Lookup Table - names
	PIMAGE_THUNK_DATA thunkIAT; // Import Address Table - addresses
	PIMAGE_IMPORT_BY_NAME nameData;

	thunkILT = (PIMAGE_THUNK_DATA)(baseAddress + importDescriptor[index].Characteristics);
	thunkIAT = (PIMAGE_THUNK_DATA)(baseAddress + importDescriptor[index].FirstThunk);
	if ((thunkIAT == NULL) or (thunkILT == NULL)) {
		return 0;
	}

	while (((*thunkILT).u1.AddressOfData != 0) & (!((*thunkILT).u1.Ordinal & IMAGE_ORDINAL_FLAG))) {
		nameData = (PIMAGE_IMPORT_BY_NAME)(baseAddress + (*thunkILT).u1.AddressOfData);
		if (!strcmp(func_to_hook, (char*)(*nameData).Name))
			break;
		thunkIAT++;
		thunkILT++;
	}

	// Hook IAT: Write over function pointer
	DWORD saved_hooked_func_addr;
	DWORD dwOld = NULL;
	saved_hooked_func_addr = (*thunkIAT).u1.Function;
	VirtualProtect((LPVOID) & ((*thunkIAT).u1.Function), sizeof(DWORD), PAGE_READWRITE, &dwOld);
	(*thunkIAT).u1.Function = new_func_address;
	VirtualProtect((LPVOID) & ((*thunkIAT).u1.Function), sizeof(DWORD), dwOld, NULL);

	return 1;
};

int main();