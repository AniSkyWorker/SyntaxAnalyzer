#include "stdafx.h"
#include "SyntaxAnalyzer.h"
#include "LRWalker.h"
#include "SyntaxExceptions.h"
#include <algorithm>

using namespace tokens;

namespace
{

InputSequence GetSequenceFromSequence(size_t currentPos, size_t length, const InputSequence & inputSeq)
{
	size_t newLenght = currentPos + length;
	if (newLenght <= inputSeq.size())
	{
		return InputSequence(inputSeq.begin() + currentPos, inputSeq.begin() + newLenght);
	}
	else
	{
		return InputSequence();
	}
}

bool CheckOneTokenExpr(const std::string & token, const InputSequence & seq, bool exceptions = false)
{
	if (seq.size() != 1 || token != seq.front())
	{
		if (exceptions)
		{
			throw ExpectedSymbolError(seq, { token });
		}
		return false;
	}
	return true;
}

std::string GetSequenceElement(const InputSequence & seq, size_t position)
{
	return position < seq.size() ? seq[position] : seq.empty() ? "" :  seq[position - 1];
}

}

CSyntaxAnalyzer::CSyntaxAnalyzer()
	: m_checkTypesMap({
		{ Types::BOOL, std::bind(&CSyntaxAnalyzer::CheckBoolExpression, this, std::placeholders::_1) },
		{ Types::INT, std::bind(&CSyntaxAnalyzer::CheckArithmeticExpression, this, std::placeholders::_1) },
		{ Types::FLOAT, std::bind(&CSyntaxAnalyzer::CheckArithmeticExpression, this, std::placeholders::_1) },
		{ Types::STRING, std::bind(&CSyntaxAnalyzer::CheckString, this, std::placeholders::_1) },
		{ Types::CHAR, std::bind(&CSyntaxAnalyzer::CheckChar, this, std::placeholders::_1) }
	})
{
}

void CSyntaxAnalyzer::CheckInputSequence(const InputSequence & inputSeq)
{
	m_currentPos = 0;
	m_inputSeq = inputSeq;
	CheckProgramStruct();
	if (m_currentPos != m_inputSeq.size())
	{
		throw UnexpectedSymbolError({ GetSequenceElement(m_inputSeq, m_currentPos) }, { GetSequenceElement(m_inputSeq, m_currentPos - 1) });
	}
}

void CSyntaxAnalyzer::CheckProgramStruct()
{
	MakeShiftIfNeeded(START_BLOCK, true, true);
	CheckMainStruct();
	MakeShiftIfNeeded(END_BLOCK, true, true);
}

void CSyntaxAnalyzer::CheckMainStruct()
{
	if (CheckWhileConstruction() || CheckIfConstruction() || CheckRead() || CheckPrint() || CheckAssignment() || CheckDeclare())
	{
		CheckMainStruct();
	}
	else if (GetNextExpressionLength() != 0)
	{
		InputSequence seq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(), m_inputSeq);
		if (GetResultOfTableCalculation(seq, TableType::arithmetic, false) || GetResultOfTableCalculation(seq, TableType::boolean, false))
		{
			m_currentPos += seq.size();
			MakeShiftIfNeeded(LINE_END, true, true);
			CheckMainStruct();
		}
	}
}

bool CSyntaxAnalyzer::CheckWhileConstruction()
{
	if (MakeShiftIfNeeded(WHILE_STATEMENT))
	{
		CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckBoolExpression, this, std::placeholders::_1));
		CheckProgramStruct();
		return true;
	}
	return false;
}

bool CSyntaxAnalyzer::CheckIfConstruction()
{	
	if (MakeShiftIfNeeded(IF_STATEMENT))
	{
		CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckBoolExpression, this, std::placeholders::_1));
		CheckProgramStruct();
		CheckElIfConstruction();
		if(MakeShiftIfNeeded(ELSE_STATMENT))
		{
			CheckProgramStruct();
		}
		return true;
	}
	return false;
}

void CSyntaxAnalyzer::CheckElIfConstruction()
{
	if (MakeShiftIfNeeded(ELIF_STATEMENT))
	{
		CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckBoolExpression, this, std::placeholders::_1));
		CheckProgramStruct();
		CheckElIfConstruction();
	}
}

bool CSyntaxAnalyzer::CheckAssignment()
{
	auto leftSeq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(ASSIGMENT), m_inputSeq);
	if (CheckTypeWithIndecies(ID, leftSeq))
	{
		m_currentPos += leftSeq.size();
		MakeShiftIfNeeded(ASSIGMENT, true, true);
		auto rightSeq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(), m_inputSeq);
		if (CheckData(rightSeq))
		{
			m_currentPos += rightSeq.size();
			MakeShiftIfNeeded(LINE_END, true, true);
			return true;
		}
	}
	return false;
}

bool CSyntaxAnalyzer::CheckBoolExpression(const InputSequence & seq)
{
	return GetResultOfTableCalculation(seq, TableType::boolean);
}

bool CSyntaxAnalyzer::CheckArithmeticExpression(const InputSequence & seq)
{
	return GetResultOfTableCalculation(seq, TableType::arithmetic);
}

bool CSyntaxAnalyzer::CheckRead()
{
	if (MakeShiftIfNeeded(READ))
	{
		CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckData, this, std::placeholders::_1));
		MakeShiftIfNeeded(LINE_END, true, true);
		return true;
	}
	return false;
}

bool CSyntaxAnalyzer::CheckPrint()
{
	if (MakeShiftIfNeeded(PRINT))
	{
		CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckData, this, std::placeholders::_1));
		MakeShiftIfNeeded(LINE_END, true, true);
		return true;
	}
	return false;
}

bool CSyntaxAnalyzer::CheckData(const InputSequence & seq)
{
	if (GetResultOfTableCalculation(seq, TableType::arithmetic, false)
		|| GetResultOfTableCalculation(seq, TableType::boolean, false)
		|| CheckTypeWithIndecies(ID, seq)
		|| CheckOneTokenExpr(type::STRING, seq)
		|| CheckOneTokenExpr(type::CHAR, seq))
	{
		return true;
	}

	throw ExpectedSymbolError(seq, { "arithmetical expression", type::STRING, type::CHAR, ID });
}

void CSyntaxAnalyzer::CheckBracketsExpr(const CheckSequenceFunc & insideBracketsExpr)
{
	MakeShiftIfNeeded(OPEN_BRACKET, true, true);
	auto foundBracketItr = std::find(m_inputSeq.begin() + m_currentPos, m_inputSeq.end(), CLOSE_BRACKET);
	if (foundBracketItr != m_inputSeq.end())
	{
		size_t exprLen = foundBracketItr - (m_inputSeq.begin() + m_currentPos);
		if (insideBracketsExpr(GetSequenceFromSequence(m_currentPos, exprLen, m_inputSeq)))
		{
			m_currentPos += exprLen;
			MakeShiftIfNeeded(CLOSE_BRACKET, true, true);
		}
	}
}

bool CSyntaxAnalyzer::CheckDeclare()
{
	bool isConst = MakeShiftIfNeeded(CONST);
	auto found = TYPES_MAP.end();

	if (m_currentPos < m_inputSeq.size())
	{
		found = std::find_if(TYPES_MAP.begin(), TYPES_MAP.end()
			, [=](const auto &typePair) {
			return typePair.second == m_inputSeq[m_currentPos];
		});
	}

	//проверка декларации id или присваивания в зависимости от типа
	if (found != TYPES_MAP.end() && MakeShiftIfNeeded(found->second))
	{
		MakeShiftIfNeeded(ID, true, true);
		auto seq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(), m_inputSeq);
		if (!seq.empty())
		{
			if (MakeShiftIfNeeded(ASSIGMENT))
			{
				seq.erase(seq.begin());
				if (!(CheckTypeWithIndecies(ID, seq) || (m_checkTypesMap.at(found->first))(seq)))
				{
					throw ExpectedSymbolError(seq, { ID });
				}
				m_currentPos += seq.size();
			}
			else if (ChecIndexStruct(seq))
			{
				m_currentPos += seq.size();
			}
		}

		MakeShiftIfNeeded(LINE_END, true, true);
		return true;
	}

	if (isConst)
	{
		throw ExpectedSymbolError({ GetSequenceElement(m_inputSeq, m_currentPos) }, { "type_name" });
	}

	return false;
}

bool CSyntaxAnalyzer::MakeShiftIfNeeded(const std::string & checkStr, bool shift, bool exception)
{
	if (m_currentPos < m_inputSeq.size())
	{
		if (m_inputSeq[m_currentPos] == checkStr)
		{
			if (shift)
			{
				m_currentPos++;
			}
			return true;
		}
	}

	if (exception)
	{
		throw ExpectedSymbolError({ GetSequenceElement(m_inputSeq, m_currentPos) }, { checkStr });
	}

	return false;
}

bool CSyntaxAnalyzer::CheckTypeWithIndecies(const std::string & type, const InputSequence & seq)
{
	return !seq.empty() && (CheckOneTokenExpr(type, seq) || (MakeShiftIfNeeded(type, false) && ChecIndexStruct(GetSequenceFromSequence(1, seq.size() - 1, seq))));
}

bool CSyntaxAnalyzer::CheckString(const InputSequence & seq)
{
	return CheckOneTokenExpr(type::STRING, seq, true);
}

bool CSyntaxAnalyzer::CheckChar(const InputSequence & seq)
{
	return CheckOneTokenExpr(type::CHAR, seq, true);
}

bool CSyntaxAnalyzer::ChecIndexStruct(const InputSequence & seq, size_t start)
{
	if (CheckIndex(seq, start))
	{
		return start == seq.size() || ChecIndexStruct(seq, start);
	}
	return  false;
}

bool CSyntaxAnalyzer::CheckIndex(const InputSequence & sequence, size_t & start)
{
	if (start < sequence.size())
	{
		if (sequence[start] == INDEX_OPEN)
		{
			start++;
			auto foundBracketItr = std::find(sequence.begin() + start, sequence.end(), INDEX_CLOSE);
			if (foundBracketItr != sequence.end())
			{
				size_t exprLen = foundBracketItr - (sequence.begin() + start);
				auto seq = GetSequenceFromSequence(start, exprLen, sequence);
				if (CheckOneTokenExpr(ID, seq) || CheckOneTokenExpr(type::INT, seq))
				{
					start += 2;
					return true;
				}
				throw ExpectedSymbolError(sequence, { ID, type::INT });
			}
			throw ExpectedSymbolError(sequence, { INDEX_CLOSE });
		}
	}
	return false;
}

size_t CSyntaxAnalyzer::GetNextExpressionLength(const std::string & separator)
{
	auto foundEndLineItr = std::find(m_inputSeq.begin() + m_currentPos, m_inputSeq.end(), separator);
	if (foundEndLineItr != m_inputSeq.end())
	{
		return foundEndLineItr - (m_inputSeq.begin() + m_currentPos);
	}

	return 0;
}

bool CSyntaxAnalyzer::GetResultOfTableCalculation(const InputSequence & seq, TableType type, bool exceptions)
{
	return m_lWalker.CheckInputSequence(seq, m_tableStorage.GetLL1Table(type), exceptions);
}
