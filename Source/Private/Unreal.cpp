#include "pch.h"
#include "Unreal.h"

namespace Paradise::Unreal {
    template<typename T>
    void TArray<T>::Free() {
        if (Data) {
            FMemory::Free(Data);
            Data = nullptr;
            NumElements = MaxElements = 0;
        }
    }

    template<typename T>
    bool TArray<T>::Remove(const int idx, int size) {
        if (idx >= NumElements) return false;
        if (idx != --NumElements) {
            Get(idx) = Get(NumElements);
        }
        return true;
    }

    void* FMemory::Malloc(size_t bytes) { 
        return IRealloc(nullptr, bytes, 0); 
    }
    
    void FMemory::Free(void* ptr) { 
        IRealloc(ptr, 0, 0); 
    }
    
    void* FMemory::Realloc(void* ptr, size_t newSize) { 
        return IRealloc(ptr, newSize, 0); 
    }

    FString::FString(const char* str) {
        if (!str) {
            Data = nullptr;
            NumElements = MaxElements = 0;
            return;
        }

        NumElements = MaxElements = static_cast<int>(strlen(str)) + 1;
        Data = static_cast<wchar_t*>(FMemory::Malloc(NumElements * sizeof(wchar_t)));
        
        size_t converted;
        mbstowcs_s(&converted, Data, NumElements, str, _TRUNCATE);
    }

    FString::FString(const wchar_t* str) {
        if (!str || !*str) {
            Data = nullptr;
            NumElements = MaxElements = 0;
            return;
        }

        NumElements = MaxElements = static_cast<int>(wcslen(str)) + 1;
        Data = static_cast<wchar_t*>(FMemory::Malloc(NumElements * sizeof(wchar_t)));
        memcpy_s(Data, NumElements * sizeof(wchar_t), str, NumElements * sizeof(wchar_t));
    }

    wchar_t* FString::ToString() {
        return Data;
    }

    FString FCurlHttpRequest::GetURL() {
        return ((FString& (*)(FCurlHttpRequest*, FString))*VTable)(this, FString());
    }

    void FCurlHttpRequest::SetURL(const wchar_t* url) {
        FString fstr(url);
        ((void (*)(FCurlHttpRequest*, FString&))VTable[10])(this, fstr);
    }
}