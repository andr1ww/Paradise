#pragma once
#include "Unreal.h"

namespace Paradise {
    constexpr auto BACKEND_URL = L"http://127.0.0.1:3551";
    constexpr auto SHOW_CONSOLE = true;
    inline bool (*ProcessRequestOG)(Paradise::Unreal::FCurlHttpRequest*);  
    
    namespace Strings {
        constexpr auto ProcessRequest = L"Could not set libcurl options for easy handle, processing HTTP request failed. Increase verbosity for additional information.";
        constexpr auto ProcessRequestStat = L"STAT_FCurlHttpRequest_ProcessRequest.";
        constexpr auto ProcessRequestFail = L"ProcessRequest failed. URL '%s' is not a valid HTTP request. %p";
        constexpr auto Realloc = L"AbilitySystem.Debug.NextTarget";
    }
}