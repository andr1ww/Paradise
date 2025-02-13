#include "pch.h"
#include "Redirect.h"

void InternalProcessRequest(FCurlHttpRequest* Request)
{
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

    if (auto [found, end] = check(URL); found) {
        std::wstring newPath = URL.substr(end);
        std::wstring newUrl = Paradise::BACKEND_URL + newPath;
        Request->SetURL(newUrl.c_str());
    }
}

bool Paradise::Redirect::ProcessRequest(FCurlHttpRequest* Request) 
{
    InternalProcessRequest(Request);
    return ProcessRequestOG(Request);
}


bool Paradise::Redirect::EOSProcessRequest(FCurlHttpRequest* Request)
{
    InternalProcessRequest(Request);
    return EOSProcessRequestOG(Request);
}