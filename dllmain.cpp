#include "pch.h"
#include <Windows.h>
#include "Source/Public/Core.h"
#include <iostream>
#include <detours.h>

DWORD WINAPI Main(LPVOID)
{
    if (Paradise::SIG_PATCH) {
        const char* sigs[] = { // idk which is for which ver so + ts is easier if i find more sigs
            "40 55 53 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B F1", 
            "40 55 53 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 33 FF 49 8B F1"
        };

        Paradise::Core::Addr = 0;

        for (const char* sig : sigs) {
            Paradise::Core::Addr = Memcury::Scanner::FindPattern(sig).Get();
            if (Paradise::Core::Addr)
                break;
        }

        if (Paradise::Core::Addr) {
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID&)Paradise::Core::Addr, (PVOID)Paradise::Core::PakPatch);
            DetourTransactionCommit();
        }
    }

    if (Paradise::SHOW_CONSOLE) {
        AllocConsole();
        FILE* fptr;
        freopen_s(&fptr, "CONOUT$", "w+", stdout);
        HWND window = GetConsoleWindow();
        if (window != nullptr) {
            SetConsoleTitleA("Paradise | https://github.com/andr1ww/Paradise");
        }
    }

    if (Paradise::USE_BACKEND_PARAM) {
        const wchar_t* cmd = GetCommandLineW();
        const wchar_t* param = L"-backend=";
        const wchar_t* found = wcsstr(cmd, param);

        if (found != nullptr) {
            size_t urlLength = wcslen(found + wcslen(param));
            wchar_t* newUrl = new wchar_t[urlLength + 1];
            wcscpy_s(newUrl, urlLength + 1, found + wcslen(param));
            Paradise::BACKEND_URL = newUrl;
        }
    }

    FMemory::IRealloc = Memcury::Scanner::FindPattern(Paradise::Strings::Realloc)
        .GetAs<decltype(FMemory::IRealloc)>();

    Sleep(2500);
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
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
