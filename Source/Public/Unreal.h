#pragma once
#include "pch.h"
#include <functional>
#include <string>
#include <cstring>

namespace Paradise::Unreal 
{
    class FMemory {
    public:
        static inline void* (*IRealloc)(void*, size_t, int64_t);
        
        static void* Malloc(size_t bytes);
        static void Free(void* ptr);
        static void* Realloc(void* ptr, size_t newSize);
    };

    template<typename ElementType>
    class TArray {
    protected:  
        ElementType* Data;
        int32_t NumElements;
        int32_t MaxElements;

    public:
        TArray() : Data(nullptr), NumElements(0), MaxElements(0) {}
        auto GetData() { return Data; }
        void Free();
        int Num() const { return NumElements; }
        auto& Get(const int idx) { return Data[idx]; }
        bool Remove(const int idx, int elementSize = sizeof(ElementType));
        
        ElementType& operator[](int i) { return Get(i); }
        const ElementType& operator[](int i) const { return Get(i); }
    };

    class FString : public TArray<wchar_t> { 
    public:
        FString() : TArray<wchar_t>() {}
        FString(const char* str);
        FString(const wchar_t* str);
        
        wchar_t* ToString();
    };

    class FCurlHttpRequest {
    public:
        void** VTable;

        FString GetURL();
        void SetURL(const wchar_t* url); 
    };
}