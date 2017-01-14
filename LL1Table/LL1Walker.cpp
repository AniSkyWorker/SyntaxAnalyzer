#include "stdafx.h"
#include "LL1Walker.h"

namespace
{
	bool CheckSymbolInInput(const std::string & currentSymbol, const std::vector<std::string> & inputSet)
	{
		return std::find(inputSet.begin(), inputSet.end(), currentSymbol) != inputSet.end();
	}

	std::string GetSequenceSymbol(const std::vector<std::string> & expectedSym)
	{
		std::string expectedSymbols;
		if (expectedSym.size() > 0)
		{
			for (size_t i = 0; i < expectedSym.size(); i++)
			{
				expectedSymbols += expectedSym[i];
				if (i + 1 < expectedSym.size())
				{
					expectedSymbols += "|";
				}
			}
		}

		return expectedSymbols;
	}
}

class CUnexpectedSymbolsError : public std::runtime_error
{
public:
	CUnexpectedSymbolsError(const std::vector<std::string>& expectedSym, const std::string & unexpectedSym, size_t index)
		: runtime_error("Error. Expected [" + GetSequenceSymbol(expectedSym) + "] but (" + unexpectedSym +"[" + std::to_string(index + 1) + "]" + ") was given!")
	{}
};

class CNoTransitionError : public std::runtime_error
{
public:
	CNoTransitionError(const std::vector<std::string>& noTransitionRuleSequence)
		: runtime_error("No transition from [" + GetSequenceSymbol(noTransitionRuleSequence) + "]")
	{}
};

bool LL1Walker::CheckInputSequence(const std::vector<std::string>& inputStr, const LL1Table & table)
{
	while (!m_stack.empty())
	{
		m_stack.pop();
	}

	size_t currentSymbolIndex = 0;
	size_t tableRowIndex = 0;
	LL1TableString currentTableRow = table[tableRowIndex];
	std::string currentSymbol = inputStr[currentSymbolIndex];

	for (;!(table[tableRowIndex].end && currentSymbolIndex == inputStr.size() - 1);
		currentTableRow = table[tableRowIndex], currentSymbol = inputStr[currentSymbolIndex])
	{
		if (CheckSymbolInInput(currentSymbol, currentTableRow.input))
		{
			if (currentTableRow.shift)
			{
				currentSymbolIndex++;
			}

			if (currentTableRow.stack)
			{
				m_stack.push(tableRowIndex + 1);
			}

			tableRowIndex = GetCurrentTransition(currentTableRow);
		}
		else if (!currentTableRow.error)
		{
			tableRowIndex++;
		}
		else
		{
			throw CUnexpectedSymbolsError(table[tableRowIndex].input, currentSymbol, currentSymbolIndex);
		}
	}
	
	//TODO чистить стэк
	return true;
}

size_t LL1Walker::GetCurrentTransition(const LL1TableString & row)
{
	if (row.transition != -1)
	{
		return row.transition;
	}
	else if (!m_stack.empty())
	{
		size_t stack_top = m_stack.top();
		m_stack.pop();
		return stack_top;
	}
	else
	{
		throw CNoTransitionError(row.input);
	}
}
