#include "pch.h"
#include "Unreal.h"

namespace Paradise {
	namespace Unreal {
		template <typename T>
		TArray<T>::TArray() : Data(nullptr), NumElements(0), MaxElements(0) {}

		template <typename T>
		void TArray<T>::Free() {
			FMemory::Free(Data);
			Data = nullptr;
			NumElements = 0;
			MaxElements = 0;
		}

		template <typename T>
		auto TArray<T>::GetData() {
			return Data;
		}

		template <typename T>
		int TArray<T>::Num() const {
			return NumElements;
		}

		template <typename T>
		auto& TArray<T>::Get(const int Index) {
			return Data[Index];
		}

		template <typename T>
		T& TArray<T>::operator[](int i) {
			return Get(i);
		}

		template <typename T>
		const T& TArray<T>::operator[](int i) const {
			return Get(i);
		}

		template <typename T>
		bool TArray<T>::Remove(const int Index, int Size) {
			if (Index < NumElements) {
				if (Index != NumElements - 1)
					Get(Index) = Get(NumElements - 1);

				--NumElements;
				return true;
			}
			return false;
		}

		void FMemory::Free(void* Data) {
			IRealloc(Data, 0, 0);
		}

		void* FMemory::Malloc(size_t Size) {
			return IRealloc(0, Size, 0);
		}

		void* FMemory::Realloc(void* Data, size_t NewSize) {
			return IRealloc(Data, NewSize, 0);
		}

		FString::FString() {
			Data = nullptr;
			NumElements = 0;
			MaxElements = 0;
		}

		FString::FString(const char* Other) {
			if (Other) {
				auto NumCharacters = (int)std::strlen(Other);
				MaxElements = NumElements = NumCharacters + 1;

				Data = static_cast<wchar_t*>(FMemory::Malloc(NumElements * sizeof(wchar_t)));

				size_t ConvertedChars = 0;
				mbstowcs_s(&ConvertedChars, Data, NumElements, Other, _TRUNCATE);
			}
			else {
				MaxElements = NumElements = 0;
				Data = nullptr;
			}
		}

		FString::FString(const wchar_t* Other) {
			MaxElements = NumElements = *Other ? (int)std::wcslen(Other) + 1 : 0;

			if (NumElements && Other) {
				Data = static_cast<wchar_t*>(FMemory::Malloc(NumElements * 2));
				memcpy_s(Data, NumElements * 2, Other, NumElements * 2);
			}
		}

		wchar_t* FString::ToString() {
			return Data;
		}

		FString FCurlHttpRequest::GetURL() {
			return ((FString & (*)(FCurlHttpRequest*, FString)) * VTable)(this, FString());
		}

		void FCurlHttpRequest::SetURL(FString url) {
			return ((void (*)(FCurlHttpRequest*, FString&)) VTable[10])(this, url);
		}
	}
}