//function definitions for winapi interface

#include <stdio.h>

#include "winapi_interface.h"
#include "codes.h"

//call OpenProcess on the target process, return the handle
HANDLE open_target_process(int pid)
{
	HANDLE handle;
	printf("[+] Opening a handle on the target process\n");
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);

	if (handle == NULL)
	{
		fprintf(stderr, "[!] Error opening target process\n");
		exit(OPEN_PROCESS_ERROR);
	}

	return handle;
}

//allocate memory for the DLL inside the target process
LPVOID allocate_process_memory(HANDLE handle, char dll[])
{
	LPVOID rb;
	unsigned int dll_length = strlen(dll) + 1;
	printf("[+] Allocating memory in target process for DLL\n");
	rb = VirtualAllocEx(handle, NULL, dll_length, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
	printf("RB1: %p\n", rb);
	if (rb == NULL)
	{
		fprintf(stderr, "[!] Error allocating memory in target process\n");
		exit(MEM_ALLOC_ERROR);
	}

	return rb;
}

//write the DLL into the target process memory
void write_process_memory(HANDLE handle, char dll[], LPVOID rb)
{
	BOOL success;
	unsigned int dll_length = strlen(dll) + 1;
	printf("[+] Writing DLL into allocated memory\n");
	success = WriteProcessMemory(handle, rb, dll, dll_length, NULL);

	if (!success)
	{
		fprintf(stderr, "[!] WriteProcessMemory failed\n");
		exit(WRITE_PROCESS_MEM_ERROR);
	}
}

//get the address of the LoadLibrary(A/W) function
VOID *get_load_library_address()
{
	printf("[+] Resolving address of LoadLibraryA\n");
	HMODULE hKernel32 = GetModuleHandle("Kernel32");
	VOID *lb;
	lb = GetProcAddress(hKernel32, "LoadLibraryA");
	printf("FIRST: %p\n", lb);

	if (lb == NULL)
	{
		fprintf(stderr, "[!] Error getting address of LoadLibraryW\n");
		exit(GET_ADDR_ERROR);
	}

	return lb;
}

HANDLE spawn_remote_thread(HANDLE handle, VOID* lb, LPVOID rb)
{
	HANDLE remote_thread;
	printf("[+] Creating remote thread to run DLL\n");
	remote_thread = CreateRemoteThread(handle, NULL, 0, (LPTHREAD_START_ROUTINE)lb, rb, 0, NULL);

	printf("Remote thread: %p\n", remote_thread);

	if (remote_thread == NULL)
	{
		fprintf(stderr, "[!] Error creating remote thread\n");
		exit(REMOTE_THREAD_ERROR);
	}

	return remote_thread;
}