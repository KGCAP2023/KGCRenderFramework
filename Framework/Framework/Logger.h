#pragma once
#include "..\pch.h"
#include "StringUtil.h"

class Logger
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void Log(std::wstring message);
	static void Log(HRESULT hr, std::wstring message);
};

