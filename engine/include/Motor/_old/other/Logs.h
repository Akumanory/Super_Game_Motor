#pragma once

#include <string>
#include <winerror.h>
#include "COMException.h"

class Logs
{
public:
	static void Debug(std::string log_str);
	static void Debug(std::wstring log_str);
	static void Error(std::string log_str);
	static void Error(std::wstring log_str);
	static void Error(HRESULT hr, std::string log_str);
	static void Error(HRESULT hr, std::wstring log_str);
	static void Error(COMException& ex);
};