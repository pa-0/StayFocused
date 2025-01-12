// 32BitHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <Tlhelp32.h>

int getModuleAddress(DWORD proc, const char *modname);

 int main(int argc, char **argv) {
    if (argc == 4 && strcmp(argv[1], "dll_handle") == 0) {
        char * dllName = argv[2];
        char * functionName = argv[3];

        HMODULE dllHandle = GetModuleHandle(dllName);
        int address = (unsigned int)GetProcAddress(dllHandle, functionName);
        printf("%s %s %d\n", dllName, functionName, address);

        return address;
    } else if (argc == 4 && strcmp(argv[1], "dll_address") == 0) {
        int pid = atoi(argv[2]);
        char * dllName = argv[3];

        int address = getModuleAddress(pid, dllName);
        printf("%s %s %d\n", argv[2], dllName, address);

        return address;
    } else {
        printf("Incorrect usage\n");
        return 0;
    }
}

int getModuleAddress(DWORD proc, const char *modname)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proc);

    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    MODULEENTRY32 mod;
    mod.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(snapshot, &mod)) {
        if (strcmp(mod.szModule, modname) == 0) {
            return (int)mod.modBaseAddr;
        }

        while (Module32Next(snapshot, &mod)) {
            if (strcmp(mod.szModule, modname) == 0) {
                return (int)mod.modBaseAddr;
            }
        }

        return 0;
    } else {
        return 0;
    }
}
