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
	/// �ܼ� �α�
	/// </summary>
	/// <param name="hr"></param>
	/// <param name="message"></param>
	static void Log(std::string message);
	/// <summary>
	/// �ܼ� �α�
	/// </summary>
	/// <param name="log"></param>
	static void Log(HRESULT hr, std::string message);
	/// <summary>
	/// �ܼ� �α�
	/// </summary>
	/// <param name="message"></param>
	static void Log(std::wstring message);
	/// <summary>
	/// �ܼ� �α�
	/// </summary>
	/// <param name="hr"></param>
	/// <param name="message"></param>
	static void Log(HRESULT hr, std::wstring message);

	/// <summary>
	/// Debug View �� �α׸� �Է��մϴ�. 
	/// </summary>
	/// <param name="log"></param>
	static void AddLog(std::string log)
	{
		logBuffer.push_back(log);
	}

	static std::vector<std::string>& GetLogBuffer() { return logBuffer; };
	static std::vector<std::string> logBuffer;
};

