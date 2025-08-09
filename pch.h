// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include "Source/Globals.h"
#include "Source/Utilities/memcury.h"
#include "Source/Public/Unreal.h"
#include <thread>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <intrin.h>
#include <sstream>
#include <array>
#include <tlhelp32.h>
#include <future>
#include <iomanip>  
#include <set>
#define IMAGEBASE *(uint64_t*)(__readgsqword(0x60) + 0x10);

using namespace Paradise::Unreal;

#define Log(...) do { \
    std::ostringstream ss; \
    ss << __VA_ARGS__; \
    printf("[Paradise] %s (%s:%d) --> %s\n", __func__, __FILE__, __LINE__, ss.str().c_str()); \
} while(0)

#endif //PCH_H
