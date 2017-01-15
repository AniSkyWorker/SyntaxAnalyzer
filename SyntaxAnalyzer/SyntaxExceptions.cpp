#include "stdafx.h"
#include "SyntaxExceptions.h"

std::string TokensToStr(const std::vector<std::string> & tokens)
{
	std::string result = "\"";
	for (auto & token : tokens)
	{
		result += " " + token;
	}
	result.erase(result.begin() + 1);
	result += "\"";

	return result;
}

std::string ExpectedTokensToStr(const std::vector<std::string>& tokens)
{
	std::string result = "\"";
	for (auto & token : tokens)
	{
		result += "|" + token;
	}
	result.erase(result.begin() + 1);
	result += "\"";

	return result;
}
