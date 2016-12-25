#include "stdafx.h"
#include "SyntaxAnalyzer.h"
#include <algorithm>

namespace
{
const std::string START_BLOCK = "{";
const std::string END_BLOCK = "}";
const std::string OPEN_BRACKET = "(";
const std::string CLOSE_BRACKET = ")";
const std::string IF_STATEMENT = "if";
const std::string WHILE_STATEMENT = "while";
//const std::string WHILE = "{";
//const std::string START_BLOCK = "{";
//const std::string START_BLOCK = "{";
}

bool CSyntaxAnalyzer::CheckInputSequence(const std::vector<std::string>& inputSeq)
{
	m_inputSeq = inputSeq;
	return CheckProgramStruct();
}

bool CSyntaxAnalyzer::CheckProgramStruct()
{
	if (MakeShiftIfNeeded(START_BLOCK))
	{
		return CheckMainStruct() || MakeShiftIfNeeded(END_BLOCK); //TODO или допускает неккоректный if или нельзя впилить пустой
	}
	return false;
}

bool CSyntaxAnalyzer::CheckMainStruct()
{
	if (CheckWhileConstruction() || CheckAssignment() || CheckIfConstruction())
	{
		return CheckProgramStruct();
	}
	return false;
}
bool CSyntaxAnalyzer::CheckWhileConstruction()
{
	if (MakeShiftIfNeeded(WHILE_STATEMENT))
	{
		return CheckBracketsCondition();
	}

	return false;
}

bool CSyntaxAnalyzer::CheckIfConstruction()
{	
	if (MakeShiftIfNeeded(IF_STATEMENT))
	{
		return CheckBracketsCondition();
	}
	 
	return false;
}

bool CSyntaxAnalyzer::CheckAssignment()
{
	return false;
}

bool CSyntaxAnalyzer::BoolExpression(size_t endOfExpression)
{
	return true;
}

bool CSyntaxAnalyzer::CheckBracketsCondition()
{
	if (MakeShiftIfNeeded(OPEN_BRACKET))
	{
		auto foundBracketItr = std::find(m_inputSeq.begin() + m_currentPos, m_inputSeq.end(), CLOSE_BRACKET);
		if (foundBracketItr != m_inputSeq.end())
		{
			auto nexSymItr = foundBracketItr + 1;
			if (nexSymItr != m_inputSeq.end() && *nexSymItr == START_BLOCK)
			{
				if (BoolExpression(m_inputSeq.end() - foundBracketItr))
				{
					m_currentPos += foundBracketItr - m_inputSeq.begin() + m_currentPos;
					if (MakeShiftIfNeeded(CLOSE_BRACKET))
					{
						return CheckProgramStruct();
					}
				}
			}
		}
	}

	return false;
}

bool CSyntaxAnalyzer::MakeShiftIfNeeded(const std::string & checkStr)
{
	if (m_currentPos < m_inputSeq.size())
	{
		if (m_inputSeq[m_currentPos] == checkStr)
		{
			m_currentPos++;
			return true;
		}
	}
	return false;
}
