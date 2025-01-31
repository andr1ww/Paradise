#include "pch.h"
#include "Redirect.h"

bool Paradise::Redirect::ProcessRequest(FCurlHttpRequest* Request)
{
    const std::wstring EPICDOMAINS[] = {
        L"ol.epicgames.com",
        L"ol.epicgames.net",
        L"on.epicgames.com",
        L"game-social.epicgames.com",
        L"ak.epicgames.com"
    };

    std::wstring URL(Request->GetURL().ToString());
    
    for (const auto& domain : EPICDOMAINS)
    {
        size_t PathIndex = URL.find(domain);
        if (PathIndex != std::wstring::npos)
        {
            auto Path = URL.substr(PathIndex + domain.length());
            auto NewURL = Paradise::BACKEND_URL + Path;
            Request->SetURL(NewURL.c_str());
            break;
        }
    }
    
    return ProcessRequestOG(Request);
}