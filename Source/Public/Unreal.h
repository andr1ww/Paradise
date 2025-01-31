#pragma once
#include "pch.h"
#include <functional>
#include <string>
#include <cstring> 

namespace Paradise {
	namespace Unreal {
		class FMemory;

		template <typename T>
		class TArray {
			friend class FString;

			T* Data;
			int32_t NumElements;
			int32_t MaxElements;

		public:
			TArray();
			void Free();
			auto GetData();
			int Num() const;
			auto& Get(const int Index);
			T& operator[](int i);
			const T& operator[](int i) const;
			bool Remove(const int Index, int Size = sizeof(T));
		};

		class FMemory {
		public:
			static inline void* (*IRealloc)(void*, size_t, int64_t);

			static void Free(void* Data);
			static void* Malloc(size_t Size);
			static void* Realloc(void* Data, size_t NewSize);
		};

		class FString : private TArray<wchar_t> {
		public:
			FString();
			FString(const char* Other);
			FString(const wchar_t* Other);
			wchar_t* ToString();
		};

		class FCurlHttpRequest {
		public:
			void** VTable;

			FString GetURL();
			void SetURL(FString url);
		};
	}
}
