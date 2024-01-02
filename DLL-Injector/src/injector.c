#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string.h>

#include "codes.h"
#include "utils.h"
#include "winapi_interface.h"

//test the dll to inject
void test_dll()
{
    int ret = 0;
    wchar_t *dll = L"D:\\DLL-Injector\\messagebox.dll";

    printf("[+] Testing functionality of %S\n", dll);
    HMODULE dll_handle = LoadLibraryW(dll);
}


int main()
{
    //test function, comment this out if you dont want to test
    //test_dll();

    //get a handle on the target process
    int pid = 0;
    char* target_process = "notepad.exe";
    char dll[] = "D:\\DLL-Injector\\messagebox.dll";
    pid = get_pid_from_name(target_process);

    //if we get any error code back, terminate
    if (pid < 0)
    {
        exit(pid);
    }

    //open the target process
    HANDLE handle;
    handle = open_target_process(pid);

    //allocate memory for the DLL inside target process
    LPVOID rb;
    rb = allocate_process_memory(handle, dll);
    printf("RB: %p\n", rb);

    //write the DLL into the allocated memory
    write_process_memory(handle, dll, rb);

    // handle to kernel32 and pass it to GetProcAddress
    VOID *lb;
    lb = get_load_library_address();
    printf("%p\n", lb);

    //start remote thread for DLL in target process
    HANDLE rt;
    //rt = CreateRemoteThread(handle, NULL, 0, (LPTHREAD_START_ROUTINE)lb, rb, 0, NULL);
    rt = spawn_remote_thread(handle, lb, rb);

    //exit gracefully
    CloseHandle(handle);
    DWORD error_code = GetLastError();
    printf("%d\n", error_code);

    return 0;
}