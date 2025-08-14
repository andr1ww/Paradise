#pragma once
#include "pch.h"
#include <detours.h>

namespace Paradise::Core
{
    void Initialize();

    inline uintptr_t Addr = 0;

    static int index = 0;
    inline int64_t PakPatch(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6)
    {
        index++;
        *(DWORD*)a1 = 0;
        if (index == 27) { // 27 is 14.40 but it may be 30 for 15.30
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourDetach(&(PVOID&)Addr, (PVOID)PakPatch);
            DetourTransactionCommit();
            return a1;
        }
    }
}