#include "stdafx.h"
#include "SyntaxAnalyzer.h"
#include "LRWalker.h"
#include <algorithm>
#include "SyntaxExceptions.h"
#include<stdarg.h>

namespace
{

const std::map<Types, std::string> TYPES_MAP =
{
	{ Types::BOOL, "bool" },
	{ Types::INT, "int" },
	{ Types::CHAR, "char" },
	{ Types::STRING, "string" },
	{ Types::FLOAT, "float"}
};

const std::string START_BLOCK = "{";
const std::string END_BLOCK = "}";
const std::string OPEN_BRACKET = "(";
const std::string CLOSE_BRACKET = ")";
const std::string INDEX_OPEN = "[";
const std::string INDEX_CLOSE = "]";
const std::string IF_STATEMENT = "if";
const std::string WHILE_STATEMENT = "while";
const std::string ELIF_STATEMENT = "elif";
const std::string ELSE_STATMENT = "else";
const std::string LINE_END = ";";
const std::string PRINT = "print";
const std::string READ = "read";
const std::string ID = "id";
const std::string ASSIGMENT = "=";
const std::string CONST = "const";

InputSequence GetSequenceFromSequence(size_t currentPos, size_t length, const InputSequence & inputSeq)
{
	return InputSequence(inputSeq.begin() + currentPos, inputSeq.begin() + currentPos + length);
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
	else if (size_t nextExprLen = GetNextExpressionLength() != 0)
	{
		InputSequence seq = GetSequenceFromSequence(m_currentPos, nextExprLen, m_inputSeq);
		if (CheckArithmeticExpression(seq) /*BoolExpression(seq)*/) //todo when realized
		{
			m_currentPos += nextExprLen;
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
	if (MakeShiftIfNeeded(ID))
	{
		MakeShiftIfNeeded(ASSIGMENT, true, true);
		auto assigmentSeq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(), m_inputSeq);
		if (CheckData(assigmentSeq))
		{
			m_currentPos += assigmentSeq.size();
			MakeShiftIfNeeded(LINE_END, true, true);
			return true;
		}
	}

	return false;
}

bool CSyntaxAnalyzer::CheckBoolExpression(const InputSequence & /*seq*/)
{
	return true;//m_LL1Walker.CheckInputSequence(seq, m_tableStorage.GetLL1Table(TableType::boolean));
}

bool CSyntaxAnalyzer::CheckArithmeticExpression(const InputSequence & /*seq*/)
{
	return false;//LRWalker::CheckInputSequence(seq, m_tableStorage.GetLRTable(TableType::arithmetic));
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

//TODO выпилить бул и int они будут в таблице
bool CSyntaxAnalyzer::CheckData(const InputSequence & seq)
{
	if (CheckArithmeticExpression(seq)
		|| CheckTypeWithIndecies(ID, seq)
		|| CheckOneTokenExpr(TYPES_MAP.at(Types::STRING), seq)
		|| CheckOneTokenExpr(TYPES_MAP.at(Types::CHAR), seq)
		|| CheckOneTokenExpr(TYPES_MAP.at(Types::FLOAT), seq)
		|| CheckOneTokenExpr(TYPES_MAP.at(Types::INT), seq))
	{
		return true;
	}
	//todo вынести в функцию
	throw ExpectedSymbolError(seq, { "arithmetical expression", TYPES_MAP.at(Types::STRING), TYPES_MAP.at(Types::CHAR), TYPES_MAP.at(Types::INT), ID });
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

//todo рефакторить
bool CSyntaxAnalyzer::CheckDeclare()
{
	MakeShiftIfNeeded(CONST);

	auto found = TYPES_MAP.end();

	if (m_currentPos < m_inputSeq.size())
	{
		found = std::find_if(TYPES_MAP.begin(), TYPES_MAP.end(), [=](const auto &typePair) { return typePair.second == m_inputSeq[m_currentPos]; });
	}

	if(found != TYPES_MAP.end() && MakeShiftIfNeeded(found->second))
	{
		MakeShiftIfNeeded(ID, true, true);
		auto seq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(), m_inputSeq);
		if (MakeShiftIfNeeded(ASSIGMENT))
		{
			seq.erase(seq.begin());
			if (CheckTypeWithIndecies(ID, seq) || (m_checkTypesMap.at(found->first))(seq))
			{
				m_currentPos += seq.size();
			}
			else
			{
				throw ExpectedSymbolError(seq, { ID });
			}
		}
		else if(ChecIndexStruct(seq))
		{
			m_currentPos += seq.size();
		}

		MakeShiftIfNeeded(LINE_END, true, true);
		return true;
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
	return CheckOneTokenExpr(type, seq) || (MakeShiftIfNeeded(type, false) && ChecIndexStruct(seq, 1));
}

bool CSyntaxAnalyzer::CheckString(const InputSequence & seq)
{
	return CheckOneTokenExpr(TYPES_MAP.at(STRING), seq, true);
}

bool CSyntaxAnalyzer::CheckChar(const InputSequence & seq)
{
	return CheckOneTokenExpr(TYPES_MAP.at(CHAR), seq, true);
}

bool CSyntaxAnalyzer::ChecIndexStruct(const InputSequence & seq, size_t start)
{
	if (CheckIndex(seq, start))
	{
		size_t currentPos = m_currentPos + start;
		if (currentPos < m_inputSeq.size())
		{
			return m_inputSeq[currentPos] == LINE_END || ChecIndexStruct(seq, start);
		}
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
				if (CheckOneTokenExpr(ID, seq) || CheckOneTokenExpr(TYPES_MAP.at(INT), seq))
				{
					start += 2;
					return true;
				}

				throw ExpectedSymbolError(sequence, { ID, TYPES_MAP.at(INT) });
			}

			throw ExpectedSymbolError(sequence, { INDEX_CLOSE });
		}
		//todo в функцию 
		throw ExpectedSymbolError(start == 0 ? sequence : GetSequenceFromSequence(start, sequence.size() - start, sequence), { LINE_END });
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
