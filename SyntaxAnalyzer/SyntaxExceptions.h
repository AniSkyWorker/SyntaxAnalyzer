#pragma once
#include <string>
#include <vector>

std::string TokensToStr(const std::vector<std::string> & tokens);
std::string ExpectedTokensToStr(const std::vector<std::string> & tokens);

class ExpectedSymbolError : public std::runtime_error
{
public:
	ExpectedSymbolError(const std::vector<std::string> & tokens, const std::vector<std::string> & expectred);
};

class UnexpectedSymbolError : public std::runtime_error
{
public:
	UnexpectedSymbolError(const std::vector<std::string>& unexpected, std::string & after);
};
