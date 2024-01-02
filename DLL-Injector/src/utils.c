//utility function definitions

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#include "utils.h"
#include "codes.h"

//find the PID for the target process we are injecting into
int get_pid_from_name(char* target_process)
{
    int target_pid = 0;
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    //take the snapshot of all processes
    printf("[+] Looking for PID of process %s\n", target_process);
    printf("[+] Getting snapshot of system processes\n");
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "[!] Error getting snapshot\n");
        return SNAPSHOT_ERROR;
    }

    //set size of structure before populating it
    pe32.dwSize = sizeof(PROCESSENTRY32);

    //retrieve information about the first process
    if (!Process32First(hProcessSnap, &pe32))
    {
        fprintf(stderr, "[!] Error getting information about first process\n");
        return FIRST_PROCESS_ERROR;
    }

    do
    {
        if (strcmp(target_process, pe32.szExeFile) == 0)
        {
            target_pid = (int)pe32.th32ProcessID;
            printf("[+] Found PID: %d\n", target_pid);
            return target_pid;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    return PID_NOT_FOUND;
}