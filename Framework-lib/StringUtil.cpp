#include "pch.h"
#include "StringUtil.h"


std::wstring StringUtil::StringToWide(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}
