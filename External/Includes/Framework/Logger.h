#pragma once
#include "StringUtil.h"

class Logger
{
public:
	Logger()
	{
		logBuffer.reserve(500);
	}

	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void Log(std::wstring message);
	static void Log(HRESULT hr, std::wstring message);

	static void AddLog(std::string log)
	{
		logBuffer.push_back(log);
	}

	static std::vector<std::string>& GetLogBuffer() { return logBuffer; };
	static std::vector<std::string> logBuffer;
};

