#include "pch.h"
#include "Redirect.h"
#include "Core.h"
#include "Finder.h"
#include <detours.h>

namespace Paradise 
{
    void Core::Initialize() {
        const std::vector<uint8_t> PATTERN_1 = { 0x48, 0x81, 0xEC };
        const std::vector<uint8_t> PATTERN_2 = { 0x40 };
        const std::vector<uint8_t> PATTERN_3 = { 0x48, 0x8B, 0xC4 };

        auto scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequest);

        if (scanner.IsValid()) {
            if (!Paradise::Finder::FindPushWidget()) {
                scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequest);
                ProcessRequestOG = scanner
                    .ScanFor(PATTERN_1, false)
                    .ScanFor(PATTERN_2, false)
                    .GetAs<decltype(ProcessRequestOG)>();
            }
            else {
                scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequestStat);
                ProcessRequestOG = scanner
                    .ScanFor(PATTERN_3, false)
                    .GetAs<decltype(ProcessRequestOG)>();
            }
        }
        else {
            scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequestStat);
            ProcessRequestOG = scanner
                .ScanFor(PATTERN_3, false)
                .GetAs<decltype(ProcessRequestOG)>();
        }

        if (auto targetPtr = ProcessRequestOG) {
            auto ref = Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>();
            if (ref) {
                DWORD oldProtect;
                if (VirtualProtect(ref, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                    *ref = reinterpret_cast<void*>(Redirect::ProcessRequest);
                    VirtualProtect(ref, sizeof(void*), oldProtect, &oldProtect);
                    Log("Successfully hooked ProcessRequest");
                    Paradise::Finder::InitializeExitHook();
                    Sleep(2000);
                    auto EOSHandle = (uintptr_t) GetModuleHandleA("EOSSDK-Win64-Shipping.dll");
                    uintptr_t CurlEasySetOpt = 0;
                    do
                        CurlEasySetOpt = Memcury::Scanner::FindPattern("89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 48 83 EC 28 48 85 C9 75 08 8D 41 2B 48 83 C4 28 C3 4C", EOSHandle).Get();
                    while (!CurlEasySetOpt);
                    Redirect::CurlSetOpt = Memcury::Scanner::FindPattern("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 33 ED 49 8B F0 48 8B D9", EOSHandle).Get();
                    if (!Redirect::CurlSetOpt)
                        Redirect::CurlSetOpt = Memcury::Scanner::FindPattern("48 89 5C 24 08 48 89 6C 24 10 56 57 41 56 48 83 EC 50 33 ED 49 8B F0 8B DA 48 8B F9", EOSHandle).Get();
                    if (!Redirect::CurlSetOpt)
                        Redirect::CurlSetOpt = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC 50 33 DB 49 8B F0 48 8B F9 8B EB 81 FA", EOSHandle).Get();

                    DetourTransactionBegin();
                    DetourUpdateThread(GetCurrentThread());
                    DetourAttach(reinterpret_cast<void**>(&CurlEasySetOpt), Paradise::Redirect::CurlEasySetOpt);
                    DetourTransactionCommit();
                }
            }
        }
        else {
            *Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>() = Redirect::ProcessRequest;
            Paradise::Finder::InitializeExitHook();
            Log("Error: Failed to find ProcessRequest");
        }
    }
}
