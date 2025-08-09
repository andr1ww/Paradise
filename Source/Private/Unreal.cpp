#include "pch.h"
#include "Unreal.h"
    __declspec(noinline) bool InternalCheckBytes(void* base, int ind, const uint8_t* bytes, size_t sz, bool upwards) {
        auto offBase = (uint8_t*)(upwards ? __int64(base) - ind : __int64(base) + ind);
        for (int i = 0; i < sz; i++) {
            if (*(offBase + i) != bytes[i]) return false;
        }
        return true;
    }
    template <uint8_t... Data>
    class CheckBytes {
    public:
        constexpr static uint8_t bytes[sizeof...(Data)] = { Data... };
        void* Base;
        int Ind;
        bool Upwards;

        CheckBytes(void* base, int ind, bool upwards = false) {
            Base = base;
            Ind = ind;
            Upwards = upwards;
		}

        operator bool() {
            return InternalCheckBytes(Base, Ind, bytes, sizeof...(Data), Upwards);
        }
    };

namespace Paradise::Unreal
{
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
        return ((FString & (*)(FCurlHttpRequest*, FString))VTable[0])(this, FString());
    }

    void FCurlHttpRequest::SetURL(const wchar_t* url) {
        void* GetFunc = *this->VTable;
        static uint32_t URLIndex = 0;
        for (int i = 0; i < 100; i++) {
            auto bytes = reinterpret_cast<uint8_t*>(GetFunc);
            if (bytes[i] == 0x48 && bytes[i + 1] == 0x8D && bytes[i + 2] == 0x91) {
                URLIndex = *(uint32_t*)(__int64(GetFunc) + i + 3);
                break;
            }
        }

        if (URLIndex == 0) {
            FString fstr(url);
            ((void (*)(FCurlHttpRequest*, FString&))VTable[10])(this, fstr);
            return;
        }

        static int64_t SetUrlIndex = -1;
        if (SetUrlIndex == -1) {
            int64_t processRequestIndex = -1;

            for (int64_t i = 0; i < 100; i++) {
                auto func = this->VTable[i];
                auto bytes = reinterpret_cast<uint8_t*>(func);

                for (int j = 0; j < 100; j++) {
                    if (bytes[j] == 0x48 && bytes[j + 1] == 0x81 && bytes[j + 2] == 0xC1) {
                        SetUrlIndex = i;
                        FString fstr(url);
                        ((void (*)(FCurlHttpRequest*, FString&))VTable[SetUrlIndex])(this, fstr);
                        return;

                    }
                }
            }
        }

        if (SetUrlIndex != -1) {
            FString fstr(url);
            ((void (*)(FCurlHttpRequest*, FString&))VTable[SetUrlIndex])(this, fstr);
        }
    }
}