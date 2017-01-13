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

LRWalker::LRWalker(const std::vector<std::string>& seq, const LRTable & table)
	: m_table(table), m_inputSeq(seq)
{}

bool LRWalker::CheckInputSequence()
{
	m_transitions.push(0);
	size_t startSize = m_inputSeq.size();
	while (!m_transitions.empty())
	{
		for (size_t i = 0; i < m_table[m_transitions.top()].size(); i++)
		{
			if (m_inputSeq.empty())
			{
				if (m_table[m_transitions.top()].back().rollup)
				{
					auto rule = *m_table[m_transitions.top()].back().rollup;
					for (size_t j = 0; j < rule.size; j++)
					{
						m_transitions.pop();
						m_elements.pop();
					}
					m_inputSeq.push_back(rule.outputSym);
					m_elements.push(rule.outputSym);
					break;
				}
				else
				{
					throw CUnexpectedSymbolsError(m_table[m_transitions.top()], "", startSize - m_inputSeq.size());
				}
			}
			else
			{
				auto currentTransition = m_table[m_transitions.top()][i];
				if (currentTransition.inputSym == m_inputSeq.front())
				{
					if (currentTransition.transition != -1)
					{
						m_transitions.push(currentTransition.transition);
						if (currentTransition.shift)
						{
							m_elements.push(m_inputSeq.front());
						}
						m_inputSeq.erase(m_inputSeq.begin());
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
							m_transitions.pop();
							m_elements.pop();
						}
						m_inputSeq.insert(m_inputSeq.begin(), rule.outputSym);
						m_elements.push(rule.outputSym);
					}
					else
					{
						throw CUnexpectedSymbolsError(m_table[m_transitions.top()], m_inputSeq.front(), startSize - m_inputSeq.size());
					}
					break;
				}
				if (i >= m_table[m_transitions.top()].size() - 1)
				{
					throw CUnexpectedSymbolsError(m_table[m_transitions.top()], m_inputSeq.front(), startSize - m_inputSeq.size());
				}
			}
		}
	}
	return true;
}