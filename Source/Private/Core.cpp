#include "pch.h"
#include "Redirect.h"
#include "Core.h"

namespace Paradise {
    void Core::Initialize() {
        const std::vector<uint8_t> PATTERN_1 = { 0x48, 0x81, 0xEC };
        const std::vector<uint8_t> PATTERN_2 = { 0x40 };
        const std::vector<uint8_t> PATTERN_3 = { 0x4C, 0x8B, 0xDC };
        
        auto scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequest);
        
        if (scanner.IsValid()) {
            ProcessRequestOG = scanner
                .ScanFor(PATTERN_1, false)
                .ScanFor(PATTERN_2, false)
                .GetAs<decltype(ProcessRequestOG)>();
        } else {
            scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequestStat);
            ProcessRequestOG = scanner
                .ScanFor(PATTERN_3, false)
                .GetAs<decltype(ProcessRequestOG)>();
        }

        if (auto targetPtr = ProcessRequestOG) {
            *Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>() = Redirect::ProcessRequest;
        } else {
            Log("Error: Failed to locate ProcessRequest");
        }
    }

    void Core::Cleanup() {
        if (auto targetPtr = ProcessRequestOG) {
            *Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>() = targetPtr;
        }
    }
}