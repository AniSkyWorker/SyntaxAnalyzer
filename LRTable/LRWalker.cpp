#include "stdafx.h"
#include "LRWalker.h"

namespace
{

	std::string GetSequenceSymbol(const std::vector<RowElement> & expectedSym)
	{
		std::string expectedSymbols;
		if (expectedSym.size() > 0)
		{
			for (size_t i = 0; i < expectedSym.size(); i++)
			{
				if (expectedSym[i].transition != -1 || expectedSym[i].rollup)
				{
					expectedSymbols += expectedSym[i].inputSym;
					if (i + 1 < expectedSym.size())
					{
						expectedSymbols += "|";
					}
				}
			}
		}

		return expectedSymbols;
	}
}

class CUnexpectedSymbolsError : public std::runtime_error
{
public:
	CUnexpectedSymbolsError(const std::vector<RowElement>& expectedSym, const std::string & unexpectedSym, size_t index)
		: runtime_error("Error. Expected [" + GetSequenceSymbol(expectedSym) + "] but (" + unexpectedSym + "[" + std::to_string(index + 1) + "]" + ") was given!")
	{}
};

class CNoTransitionError : public std::runtime_error
{
public:
	CNoTransitionError(const std::vector<RowElement>& noTransitionRuleSequence)
		: runtime_error("No transition from [" + GetSequenceSymbol(noTransitionRuleSequence) + "]")
	{}
};

bool LRWalker::CheckInputSequence(std::vector<std::string> seq, const LRTable & table)
{
	std::stack<std::string> elements;
	std::stack<unsigned> transitions;
	transitions.push(0);
	size_t startSize = seq.size();
	while (!transitions.empty())
	{
		for (size_t i = 0; i < table[transitions.top()].size(); i++)
		{
			if (seq.empty())
			{
				if (table[transitions.top()].back().rollup)
				{
					auto rule = *table[transitions.top()].back().rollup;
					for (size_t j = 0; j < rule.size; j++)
					{
						transitions.pop();
						elements.pop();
					}
					seq.push_back(rule.outputSym);
					elements.push(rule.outputSym);
					break;
				}
				else
				{
					throw CUnexpectedSymbolsError(table[transitions.top()], "", startSize - seq.size());
				}
			}
			else
			{
				auto currentTransition = table[transitions.top()][i];
				if (currentTransition.inputSym == seq.front())
				{
					if (currentTransition.transition != -1)
					{
						transitions.push(currentTransition.transition);
						if (currentTransition.shift)
						{
							elements.push(seq.front());
						}
						seq.erase(seq.begin());
					}
					else if (currentTransition.shift && currentTransition.inputSym == "S")
					{
						return true;
					}
					else if (currentTransition.rollup)
					{
						auto rule = *currentTransition.rollup;
						for (size_t j = 0; j < rule.size; j++)
						{
							transitions.pop();
							elements.pop();
						}
						seq.insert(seq.begin(), rule.outputSym);
						elements.push(rule.outputSym);
					}
					else
					{
						throw CUnexpectedSymbolsError(table[transitions.top()], seq.front(), startSize - seq.size());
					}
					break;
				}
				if (i >= table[transitions.top()].size() - 1)
				{
					throw CUnexpectedSymbolsError(table[transitions.top()], seq.front(), startSize - seq.size());
				}
			}
		}
	}
	return true;
}