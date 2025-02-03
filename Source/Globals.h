#pragma once
#include "Unreal.h"

namespace Paradise {
    constexpr auto BACKEND_URL = L"http://127.0.0.1:3551";
    constexpr auto SHOW_CONSOLE = true;
    inline bool (*ProcessRequestOG)(Paradise::Unreal::FCurlHttpRequest*);  
    
    namespace Strings {
        constexpr auto ProcessRequest = L"Could not set libcurl options for easy handle, processing HTTP request failed. Increase verbosity for additional information.";
        constexpr auto ProcessRequestStat = L"STAT_FCurlHttpRequest_ProcessRequest";
        constexpr auto Realloc = "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 48 8B F1 41 8B D8 48 8B 0D ? ? ? ?";
    }
}