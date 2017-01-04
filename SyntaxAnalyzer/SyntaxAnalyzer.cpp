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
const std::string LINE_END = ";";
const std::string PRINT = "print";
const std::string READ = "read";
const std::string STRING = "string";
const std::string CHAR = "char";

InputSequence GetSequenceFromSequence(size_t currentPos, size_t length, const InputSequence & inputSeq)
{
	return InputSequence(inputSeq.begin() + currentPos, inputSeq.begin() + currentPos + length);
}

}

bool CSyntaxAnalyzer::CheckInputSequence(const std::vector<std::string>& inputSeq)
{
	m_inputSeq = inputSeq;
	return CheckProgramStruct() && m_currentPos == m_inputSeq.size();
}

bool CSyntaxAnalyzer::CheckProgramStruct()
{
	if (MakeShiftIfNeeded(START_BLOCK))
	{
		return CheckMainStruct() && MakeShiftIfNeeded(END_BLOCK);
	}

	m_state = State::Error;
	return false;
}

bool CSyntaxAnalyzer::CheckMainStruct()
{
	if (CheckWhileConstruction() || CheckIfConstruction() || CheckRead() || CheckPrint())
	{
		return CheckMainStruct();
	}
	else if (size_t nextExprLen = GetNextExpressionLength() != 0)
	{
		InputSequence seq = GetSequenceFromSequence(m_currentPos, nextExprLen, m_inputSeq);
		if (CheckAssignment(seq) || CheckArithmeticExpression(seq) /*BoolExpression(seq)*/)
		{
			m_currentPos += nextExprLen;
			return CheckMainStruct();
		}
	}

	return m_state == State::None;
}

bool CSyntaxAnalyzer::CheckWhileConstruction()
{
	if (MakeShiftIfNeeded(WHILE_STATEMENT))
	{
		if (CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::BoolExpression, this, std::placeholders::_1)))
		{
			return CheckProgramStruct();
		}
	}

	return false;
}

bool CSyntaxAnalyzer::CheckIfConstruction()
{	
	if (MakeShiftIfNeeded(IF_STATEMENT))
	{
		if (CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::BoolExpression, this, std::placeholders::_1)))
		{
			return CheckProgramStruct();
		}
	}

	return false;
}

bool CSyntaxAnalyzer::CheckAssignment(const InputSequence &)
{
	return false;//llWalker.CheckInputSequence(tableTokens, LL1Table()); // TODO хранилище
}

bool CSyntaxAnalyzer::BoolExpression(const InputSequence &n)
{
	return true;//llWalker.CheckInputSequence(tableTokens, LL1Table()); // TODO хранилище
}

bool CSyntaxAnalyzer::CheckArithmeticExpression(const InputSequence &)
{
	return false;//llWalker.CheckInputSequence(tableTokens, LL1Table()); // TODO хранилище
}

bool CSyntaxAnalyzer::CheckRead()
{
	if (MakeShiftIfNeeded(READ))
	{
		if (CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckData, this, std::placeholders::_1)))
		{
			return MakeShiftIfNeeded(LINE_END);
		}
	}
	return false;
}

bool CSyntaxAnalyzer::CheckPrint()
{
	if (MakeShiftIfNeeded(PRINT))
	{
		if (CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckData, this, std::placeholders::_1)))
		{
			return MakeShiftIfNeeded(LINE_END);
		}
	}
	return false;
}

bool CSyntaxAnalyzer::CheckData(const InputSequence & seq)
{
	return /*BoolExpression(seq) ||*/ CheckArithmeticExpression(seq) || CheckOneTokenExpr(STRING, seq) || CheckOneTokenExpr(CHAR, seq);
}

bool CSyntaxAnalyzer::CheckBracketsExpr(const std::function<bool(const InputSequence &)>& insideBracketsExpr)
{
	if (MakeShiftIfNeeded(OPEN_BRACKET))
	{
		auto foundBracketItr = std::find(m_inputSeq.begin() + m_currentPos, m_inputSeq.end(), CLOSE_BRACKET);
		if (foundBracketItr != m_inputSeq.end())
		{
			size_t exprLen = foundBracketItr - (m_inputSeq.begin() + m_currentPos);
			if (insideBracketsExpr(GetSequenceFromSequence(m_currentPos, exprLen, m_inputSeq)))
			{
				m_currentPos += exprLen;
				return MakeShiftIfNeeded(CLOSE_BRACKET);
			}
		}
	}
	m_state = State::Error;
	return false;
}

bool CSyntaxAnalyzer::ChekDeclare(const InputSequence &)
{
	return false;//llWalker.CheckInputSequence(tableTokens, LL1Table()); // TODO хранилище
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

bool CSyntaxAnalyzer::CheckOneTokenExpr(const std::string & token, const InputSequence & seq)
{
	if (seq.size() == 1)
	{
		return token == seq.front();
	}
	return false;
}

size_t CSyntaxAnalyzer::GetNextExpressionLength()
{
	auto foundEndLineItr = std::find(m_inputSeq.begin() + m_currentPos, m_inputSeq.end(), LINE_END);
	if (foundEndLineItr != m_inputSeq.end())
	{
		return foundEndLineItr - (m_inputSeq.begin() + m_currentPos);
	}

	return 0;
}
