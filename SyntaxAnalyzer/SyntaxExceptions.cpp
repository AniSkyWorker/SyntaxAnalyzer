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

ExpectedSymbolError::ExpectedSymbolError(const std::vector<std::string>& tokens, const std::vector<std::string>& expectred)
	: runtime_error("Error. Expected " + ExpectedTokensToStr(expectred) + " but given: " + TokensToStr(tokens))
{}

UnexpectedSymbolError::UnexpectedSymbolError(const std::vector<std::string>& unexpected, std::string & after)
	: runtime_error("Unexpected symbol: " + TokensToStr(unexpected) + " after: \"" + after + "\"")
{}
