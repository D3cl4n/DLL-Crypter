#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD nReason, LPVOID lpReserved)
{
	switch (nReason)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, "DLL Injection", "Evil DLL", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}