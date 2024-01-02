// wrapper functions around the needed winapi functionality for DLL injection

#ifndef WINAPI_INTERFACE_H
#define WINAPI_INTERFACE_H

#include <windows.h>

HANDLE open_target_process(int pid);
LPVOID allocate_process_memory(HANDLE handle, char dll[]);
void write_process_memory(HANDLE handle, char dll[], LPVOID rb);
VOID *get_load_library_address();
HANDLE spawn_remote_thread(HANDLE handle, VOID* lb, LPVOID rb);

#endif 