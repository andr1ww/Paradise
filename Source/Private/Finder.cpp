#include "pch.h"
#include "Finder.h"
#include <detours.h>

uintptr_t Paradise::Finder::FindPushWidget()
{
    auto p = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 48 8B E9 49 8B D9 48 8D 0D ? ? ? ? 49 8B F8 48 8B F2 E8 ? ? ? ? 4C 8B CF 48 89 5C 24 ? 4C 8B C6 48 8B D5 48 8B 48 78").Get();
    if (!p)
        p = Memcury::Scanner::FindPattern("48 8B C4 4C 89 40 18 48 89 50 10 48 89 48 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 68 B8 48 81 EC ? ? ? ? 65 48 8B 04 25").Get();
    if (!p)
        p = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 41 56 41 57 48 8D 68 A1 48 81 EC ? ? ? ? 65 48 8B 04 25 ? ? ? ? 48 8B F9 B9 ? ? ? ? 49").Get();
    return p;
}

void Paradise::Finder::InitializeExitHook()
{
    static bool bInitSet = false;
	if (bInitSet)
		return;
	bInitSet = true;
    
    auto UnsafeEnvironmentPopup = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 89 73 F0 49 89 7B E8 48 8B F9 4D 89 63 E0 4D 8B E0 4D 89 6B D8").Get();

    if (!UnsafeEnvironmentPopup)
    {
        UnsafeEnvironmentPopup = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 89").Get(); // patch4 - 19.10
    }
    if (!UnsafeEnvironmentPopup)
    {
        UnsafeEnvironmentPopup = Memcury::Scanner::FindPattern("40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? ? 0F B6 ?").Get(); // patch5 - 29.00/22.40
    }
    if (!UnsafeEnvironmentPopup)
    {
        UnsafeEnvironmentPopup = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? ? 0F B6 ? 44 88 44 24 ?").Get(); // patch7 - 28.00
    }
    if (!UnsafeEnvironmentPopup)
    {
        UnsafeEnvironmentPopup = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B DA 48 8B F1").Get(); // patch6 - 30.00
    }
    
    if (!UnsafeEnvironmentPopup)
    {
        Log("Failed to find UnsafeEnvironmentPopupAddr");
    }
    
    auto RequestExitWithStatus = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC 40 41 B9 ? ? ? ? 0F B6 F9 44 38 0D ? ? ? ? 0F B6 DA 72 24 89 5C 24 30 48 8D 05 ? ? ? ? 89 7C 24 28 4C 8D 05 ? ? ? ? 33 D2 48 89 44 24 ? 33 C9 E8 ? ? ? ?").Get();
    if (!RequestExitWithStatus)
    {
        RequestExitWithStatus = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 18 88 50 10 88 48 08 57 48 83 EC 30").Get();
        if (!RequestExitWithStatus)
            RequestExitWithStatus = Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B 08 49 89 6B 10 49 89 73 18 49 89 7B 20 41 56 48 83 EC 30 80 3D ? ? ? ? ? 49 8B").Get();
    }
    if (!RequestExitWithStatus)
    {
        Log("Failed to find RequestExitWithStatusAddr");
    }
    
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(reinterpret_cast<void**>(&UnsafeEnvironmentPopup), Paradise::UnsafeEnvironmentPopup);
    DetourTransactionCommit();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(reinterpret_cast<void**>(&RequestExitWithStatus), Paradise::RequestExitWithStatus);
    DetourTransactionCommit();
}