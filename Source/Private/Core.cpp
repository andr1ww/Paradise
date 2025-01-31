#include "pch.h"
#include "Redirect.h"
#include "Core.h"

namespace Paradise
{
    void Core::Initialize()
    {
        auto Ref = Memcury::Scanner::FindStringRef(Strings::ProcessRequest);

        if (Ref.IsValid())
        {
            ProcessRequestOG = Ref.ScanFor({ 0x48, 0x81, 0xEC }, false) .ScanFor({ 0x40 }, false).GetAs<decltype(ProcessRequestOG)>();
        }
        else
        {
            Ref = Memcury::Scanner::FindStringRef(Strings::ProcessRequestStat);

            ProcessRequestOG = Ref.ScanFor({ 0x4C, 0x8B, 0xDC }, false).GetAs<decltype(ProcessRequestOG)>();
        }

        if (ProcessRequestOG)
        {
            *Memcury::Scanner::FindPointerRef(ProcessRequestOG).GetAs<void**>() = Redirect::ProcessRequest;
        }
        else
        {
            Log("Error: Failed to locate ProcessRequest");
        }
    }

    void Core::Cleanup()
    {
        if (ProcessRequestOG)
        {
            *Memcury::Scanner::FindPointerRef(ProcessRequestOG).GetAs<void**>() = ProcessRequestOG;
        }
    }
}