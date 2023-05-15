#pragma once
#include "StringUtil.h"

class Logger
{
public:
	Logger()
	{
		logBuffer.reserve(500);
	}

	/// <summary>
	/// 콘솔 로그
	/// </summary>
	/// <param name="hr"></param>
	/// <param name="message"></param>
	static void Log(std::string message);
	/// <summary>
	/// 콘솔 로그
	/// </summary>
	/// <param name="log"></param>
	static void Log(HRESULT hr, std::string message);
	/// <summary>
	/// 콘솔 로그
	/// </summary>
	/// <param name="message"></param>
	static void Log(std::wstring message);
	/// <summary>
	/// 콘솔 로그
	/// </summary>
	/// <param name="hr"></param>
	/// <param name="message"></param>
	static void Log(HRESULT hr, std::wstring message);

	/// <summary>
	/// Debug View 에 로그를 입력합니다. 
	/// </summary>
	/// <param name="log"></param>
	static void AddLog(std::string log)
	{
		logBuffer.push_back(log);
	}

	static std::vector<std::string>& GetLogBuffer() { return logBuffer; };
	static std::vector<std::string> logBuffer;
};

