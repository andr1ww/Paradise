#include "pch.h"
#include "Redirect.h"
#include "Finder.h"

void InternalProcessRequest(FCurlHttpRequest* Request, bool bEOS)
{
    static int RequestCount = 0;
    RequestCount++;

    if (RequestCount == 10) {
        auto EOSHandle = (uintptr_t)GetModuleHandleA("EOSSDK-Win64-Shipping.dll");
        if (EOSHandle) {
            auto scanner = Memcury::Scanner::FindStringRef(L"ProcessRequest failed. URL '%s' is not using a whitelisted domain. %p", EOSHandle);
            Paradise::EOSProcessRequestOG = scanner
                .ScanFor({ 0x48, 0x89, 0x5c }, false)
                .GetAs<decltype(Paradise::EOSProcessRequestOG)>();

            auto ref = Memcury::Scanner::FindPointerRef(Paradise::EOSProcessRequestOG, EOSHandle).GetAs<void**>();
            if (ref) {
                DWORD oldProtect;
                if (VirtualProtect(ref, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                    *ref = reinterpret_cast<void*>(Paradise::Redirect::EOSProcessRequest);
                    VirtualProtect(ref, sizeof(void*), oldProtect, &oldProtect);
                    Log("Successfully hooked EOS ProcessRequest");
                }
            }
        }
    }

    auto check = [](const std::wstring& fullUrl) -> std::pair<bool, size_t> {
        const auto domains = {
            std::wstring(L"game-social.epicgames.com"),
            std::wstring(L"ol.epicgames.com"),
            std::wstring(L"ol.epicgames.net"),
            std::wstring(L"on.epicgames.com"),
            std::wstring(L"ak.epicgames.com"),
            std::wstring(L"epicgames.dev")
        };

        for (const auto& d : domains) {
            if (size_t pos = fullUrl.find(d); pos != std::wstring::npos) {
                return { true, pos + d.length() };
            }
        }
        return { false, 0 };
        };

    std::wstring URL = Request->GetURL().ToString();
    static bool bInitExit = false;
    if (auto [found, end] = check(URL); found) {
        std::wstring newPath = URL.substr(end);
        if (newPath.find(L"dedicated_server") != std::wstring::npos) {
            if (!bInitExit) {
                bInitExit = true;
                Paradise::Finder::InitializeExitHook();
            }
        }
        std::wstring newUrl = Paradise::BACKEND_URL + newPath;
        if (!bEOS) {
            Request->SetURL(newUrl.c_str());
        }
        else {
			Request->SetURL(newUrl.c_str(), true);
        }
    }
}

bool Paradise::Redirect::ProcessRequest(FCurlHttpRequest* Request) 
{
    InternalProcessRequest(Request, false);
    return ProcessRequestOG(Request);
}


bool Paradise::Redirect::EOSProcessRequest(FCurlHttpRequest* Request)
{
    InternalProcessRequest(Request, true);
    return EOSProcessRequestOG(Request);
}