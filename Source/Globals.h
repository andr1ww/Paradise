#pragma once
#include "Unreal.h"

namespace Paradise {
    inline const wchar_t* BACKEND_URL = L"http://127.0.0.1:3551";
    constexpr auto SHOW_CONSOLE = true;
    constexpr auto USE_BACKEND_PARAM = true;

    inline bool (*EOSProcessRequestOG)(Paradise::Unreal::FCurlHttpRequest*);
    inline bool (*ProcessRequestOG)(Paradise::Unreal::FCurlHttpRequest*);
    inline void RequestExitWithStatus(bool Force, unsigned char Code) {}

    inline void UnsafeEnvironmentPopup(wchar_t** unknown1, unsigned __int8* case_, __int64* unknown2, char unknown3) {};

    namespace Strings {
        constexpr auto ProcessRequest = L"Could not set libcurl options for easy handle, processing HTTP request failed. Increase verbosity for additional information.";
        constexpr auto ProcessRequestStat = L"STAT_FCurlHttpRequest_ProcessRequest";
        constexpr auto Realloc = "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 48 8B F1 41 8B D8 48 8B 0D ? ? ? ?";
    }
}