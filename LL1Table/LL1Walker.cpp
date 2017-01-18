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
	CUnexpectedSymbolsError(const std::vector<std::string>& expectedSym, const std::string & unexpectedSym)
		: runtime_error("Error. Expected [" + GetSequenceSymbol(expectedSym) + "] but (" + unexpectedSym + ") was given!")
	{}
};

class CNoTransitionError : public std::runtime_error
{
public:
	CNoTransitionError(const std::vector<std::string>& noTransitionRuleSequence, size_t rowCount)
		: runtime_error("No transition from [" + GetSequenceSymbol(noTransitionRuleSequence) + "]" + "(" + std::to_string(rowCount) + ")")
	{}
};

bool LL1Walker::CheckInputSequence(const std::vector<std::string>& inputStr, const LL1Table & table, bool except)
{
	try
	{
		while (!m_stack.empty())
		{
			m_stack.pop();
		}

		if (inputStr.empty())
		{
			return false;
		}

		size_t currentSymbolIndex = 0;
		size_t tableRowIndex = 0;
		LL1TableString currentTableRow = table[tableRowIndex];
		std::string currentSymbol = inputStr[currentSymbolIndex];

		for (;currentSymbolIndex < inputStr.size(); currentTableRow = table[tableRowIndex], currentSymbol = inputStr[currentSymbolIndex])
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

				if (currentSymbolIndex == inputStr.size())
				{
					//todo close brackets
					if (table[tableRowIndex].end)
					{
						return true;
					}
					else
					{
						throw CUnexpectedSymbolsError(table[GetCurrentTransition(currentTableRow, tableRowIndex)].input, "expression end");
					}
				}

				tableRowIndex = GetCurrentTransition(currentTableRow, tableRowIndex);
			}
			else if (!currentTableRow.error)
			{
				tableRowIndex++;
			}
			else
			{
				if (tableRowIndex != 0)
				{
					throw CUnexpectedSymbolsError(table[tableRowIndex].input, currentSymbol);
				}
				return false;
			}
		}
		return false;
	}
	catch (const std::exception& exc)
	{
		if (except)
		{
			throw exc;
		}
		return false;
	}


}

size_t LL1Walker::GetCurrentTransition(const LL1TableString & row, size_t currentRowCount)
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
		throw CNoTransitionError(row.input, currentRowCount);
	}
}
