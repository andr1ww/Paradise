#include "pch.h"
#include "Redirect.h"
#include "Core.h"
#include "Finder.h"

namespace Paradise
{
    void Core::Initialize()
    {
        Memcury::Scanner scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequestStat, Memcury::PE::GetModuleBase(), false);
        
        if (!scanner.IsValid())
            scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequest, Memcury::PE::GetModuleBase(), false);
        
        if (!scanner.IsValid())
        {
            Sleep(2500);
            scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequestStat, Memcury::PE::GetModuleBase(), false);
        
            if (!scanner.IsValid())
                scanner = Memcury::Scanner::FindStringRef(Strings::ProcessRequest, Memcury::PE::GetModuleBase(), false);
            if (!scanner.IsValid())
            {
                Finder::InitializeExitHook();
                return;
            }
        }
        
        uint8_t* stream = scanner.GetAs<uint8_t*>();
        void* FunctionPtr = nullptr;
        
        for (int i = 0; i < 2048; i++)
        {
            uint8_t* current = stream - i;
            
            bool isPattern1 = current[0] == 0x4C && current[1] == 0x8B && current[2] == 0xDC;
            bool isPattern2 = current[0] == 0x48 && current[1] == 0x8B && current[2] == 0xC4;
            
            if (isPattern1)
            {
                FunctionPtr = current;
                break;
            }
            else if (isPattern2)
            {
                FunctionPtr = current;
                break;
            }
            else
            {
                bool isPattern3 = current[0] == 0x48 && current[1] == 0x81 && current[2] == 0xEC;
                bool isPattern4 = current[0] == 0x48 && current[1] == 0x83 && current[2] == 0xEC;
                
                if (isPattern3 || isPattern4)
                {
                    for (int x = 0; x < 50; x++)
                    {
                        uint8_t* inner = stream - i - x;
                        
                        if (inner[0] == 0x40)
                        {
                            FunctionPtr = inner;
                            goto _found;
                        }
                        else
                        {
                            bool innerPattern1 = inner[0] == 0x4C && inner[1] == 0x8B && inner[2] == 0xDC;
                            bool innerPattern2 = inner[0] == 0x48 && inner[1] == 0x8B && inner[2] == 0xC4;
                            bool innerPattern3 = inner[0] == 0x48 && inner[1] == 0x89 && inner[2] == 0x5C;
                            
                            if (innerPattern1 || innerPattern2 || innerPattern3)
                                break;
                        }
                    }
                }
            }
        }

    _found:
        if (!FunctionPtr)
        {
            Finder::InitializeExitHook();
            return;
        }

        ProcessRequestOG = reinterpret_cast<decltype(ProcessRequestOG)>(FunctionPtr);
        void** ref = Memcury::Scanner::FindPtrRef(ProcessRequestOG).GetAs<void**>();
        
        if (!ref)
        {
            Finder::InitializeExitHook();
            return;
        }
 
        DWORD oldProtect;
        
        if (VirtualProtect(ref, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            *ref = reinterpret_cast<void*>(Redirect::ProcessRequest);
            VirtualProtect(ref, sizeof(void*), oldProtect, &oldProtect);
        }
    }
}