#include "pch.h"
#include "Logger.h"
#include <comdef.h>

std::vector<std::string> Logger::logBuffer;
//MessageBoxA ¿Í MessageBoxW

void Logger::Log(std::string message)
{
	std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void Logger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + StringUtil::StringToWide(message) + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void Logger::Log(std::wstring message)
{
	std::wstring error_message = L"Error: " + message + L"\n";
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void Logger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}



