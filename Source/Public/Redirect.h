#pragma once
#include "pch.h"

namespace Paradise::Redirect
{
	inline uintptr_t CurlSetOpt = 0;
	CURLcode CurlEasySetOpt(void*, CURLoption, ...);
	bool ProcessRequest(FCurlHttpRequest* Request);
}