#include "pch.h"
#include <Windows.h>
#include "Source/Public/Core.h"
#include <iostream>

DWORD WINAPI Main(LPVOID)
{
    if (Paradise::SHOW_CONSOLE) {
        AllocConsole();
        FILE* fptr;
        freopen_s(&fptr, "CONOUT$", "w+", stdout);
		SetWindowTextA(GetConsoleWindow(), "Paradise | https://github.com/itztiva/Paradise");
    }

    FMemory::IRealloc = Memcury::Scanner::FindPattern(Paradise::Strings::Realloc)
        .GetAs<decltype(FMemory::IRealloc)>();

    Paradise::Core::Initialize();

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

