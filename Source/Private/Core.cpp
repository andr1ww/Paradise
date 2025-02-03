#include "pch.h"
#include "Redirect.h"
#include "Core.h"
#include "Finder.h"

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
                }
            }
        }
        else {
            *Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>() = Redirect::ProcessRequest;
            Log("Error: Failed to find ProcessRequest");
        }
    }
}
