#include "pch.h"
#include "Redirect.h"
#include "Core.h"
#include "Finder.h"
#include <detours.h>

namespace Paradise
{
    void Core::Initialize()
    {
        const std::vector<uint8_t> PATTERN_1 = {0x48, 0x81, 0xEC};
        const std::vector<uint8_t> PATTERN_2 = {0x40};
        const std::vector<uint8_t> PATTERN_3 = {0x48, 0x8B, 0xC4};
        const std::vector<uint8_t> PATTERN_4 = {0x4C, 0x8B, 0xDC};
        const std::vector<uint8_t> PATTERN_5 = {0x48, 0x83, 0xEC};

        auto scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequestStat, Memcury::PE::GetModuleBase(), false);

        if (!scanner.IsValid())
        {
            scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequest);
            ProcessRequestOG = scanner
                                   .ScanFor(PATTERN_1, false)
                                   .ScanFor(PATTERN_2, false)
                                   .GetAs<decltype(ProcessRequestOG)>();
        }
        else
        {
            ProcessRequestOG = scanner
                                   .ScanFor(PATTERN_1, false)
                                   .ScanFor(PATTERN_2, false)
                                   .GetAs<decltype(ProcessRequestOG)>();
        }

        if (auto targetPtr = ProcessRequestOG)
        {
            auto ref = Memcury::Scanner::FindPtrRef(targetPtr).GetAs<void **>();
            if (!ref)
            {
                if (scanner.IsValid())
                {
                    ProcessRequestOG = scanner
                                           .ScanFor(PATTERN_5, false)
                                           .ScanFor(PATTERN_2, false)
                                           .GetAs<decltype(ProcessRequestOG)>();
                    ref = Memcury::Scanner::FindPtrRef(targetPtr).GetAs<void **>();
                    if (!ref)
                    {
                        ProcessRequestOG = scanner
                                               .ScanFor(PATTERN_3, false)
                                               .GetAs<decltype(ProcessRequestOG)>();
                        ref = Memcury::Scanner::FindPtrRef(targetPtr).GetAs<void **>();
                        if (!ref)
                        {
                            ProcessRequestOG = scanner
                                                   .ScanFor(PATTERN_4, false)
                                                   .GetAs<decltype(ProcessRequestOG)>();
                        }
                    }
                }
            }

            if (ref)
            {
                DWORD oldProtect;
                if (VirtualProtect(ref, sizeof(void *), PAGE_EXECUTE_READWRITE, &oldProtect))
                {
                    *ref = reinterpret_cast<void *>(Redirect::ProcessRequest);
                    VirtualProtect(ref, sizeof(void *), oldProtect, &oldProtect);
                    Log("Successfully hooked ProcessRequest");
                }
            }
        }
        else
        {
            *Memcury::Scanner::FindPtrRef(targetPtr).GetAs<void **>() = Redirect::ProcessRequest;
            Paradise::Finder::InitializeExitHook();
            Log("Error: Failed to find ProcessRequest");
        }
    }
}
