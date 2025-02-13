#include "pch.h"
#include "Redirect.h"


inline CURLcode SetOpt(void* Data, CURLoption Tag, ...)
{
    va_list arg;
    va_start(arg, Tag);

    CURLcode(*CurlSetOptInternal)(void*, CURLoption, va_list) = decltype(CurlSetOptInternal)(Paradise::Redirect::CurlSetOpt);
    const auto result = CurlSetOptInternal(Data, Tag, arg);

    va_end(arg);
    return result;
}

CURLcode Paradise::Redirect::CurlEasySetOpt(void* Data, CURLoption Tag, ...) {
    va_list arg;
    va_start(arg, Tag);

    CURLcode(*CurlSetOptInternal)(void*, CURLoption, va_list) = decltype(CurlSetOptInternal)(Paradise::Redirect::CurlSetOpt);
    CURLcode res{};
    if (Tag == CURLOPT_SSL_VERIFYPEER)
        res = SetOpt(Data, Tag, 0);
    else if (Tag == CURLOPT_URL) {
        std::string url = va_arg(arg, char*);

        auto check = [](const std::string& fullUrl) -> std::pair<bool, size_t> {
            const auto domains = {
                std::string("epicgames.dev"),
            };

            for (const auto& d : domains) {
                if (size_t pos = fullUrl.find(d); pos != std::string::npos) {
                    return { true, pos + d.length() };
                }
            }
            return { false, 0 };
            };


        if (auto [found, end] = check(url); found) {
            std::string newPath = url.substr(end);
            url = Paradise::BACKEND_URL + newPath;
        }

        res = SetOpt(Data, Tag, url.c_str());
    }
    else
        res = CurlSetOptInternal(Data, Tag, arg);

    va_end(arg);
    return res;
}
bool Paradise::Redirect::ProcessRequest(FCurlHttpRequest* Request) 
{
    auto check = [](const std::wstring& fullUrl) -> std::pair<bool, size_t> {
        const auto domains = {
            std::wstring(L"game-social.epicgames.com"),
            std::wstring(L"ol.epicgames.com"),
            std::wstring(L"ol.epicgames.net"),
            std::wstring(L"on.epicgames.com"),
            std::wstring(L"ak.epicgames.com")
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
        std::string backendUrl = Paradise::BACKEND_URL;
        std::wstring newUrl = std::wstring(backendUrl.begin(), backendUrl.end()) + newPath;
        Request->SetURL(newUrl.c_str());
    }
    
    return ProcessRequestOG(Request);
}