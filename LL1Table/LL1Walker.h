#pragma once
#include "LL1TableString.h"
#include <stack>

class LL1Walker
{
public:
	bool CheckInputSequence(const std::vector<std::string>& inputStr, const LL1Table & table);

private:
	size_t GetCurrentTransition(const LL1TableString & row, unsigned currentRowCount);

	std::stack<size_t> m_stack;
};
