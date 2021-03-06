#pragma once
#include "LL1TableString.h"
#include <stack>

class LL1Walker
{
public:
	bool CheckInputSequence(const std::vector<std::string>& inputStr, const LL1Table & table, bool except = true);

private:
	size_t GetCurrentTransition(const LL1TableString & row, size_t currentRowCount);

	std::stack<size_t> m_stack;
};
