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
const std::string ID = "id";
const std::string ASSIGMENT = "=";

const std::map<Types, std::string> TYPES_MAP =
{
	{ Types::BOOL, "bool" },
	{ Types::INT, "int" },
	{ Types::CHAR, "char" },
	{ Types::STRING, "string" },
	{ Types::FLOAT, "float"}
};

InputSequence GetSequenceFromSequence(size_t currentPos, size_t length, const InputSequence & inputSeq)
{
	return InputSequence(inputSeq.begin() + currentPos, inputSeq.begin() + currentPos + length);
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

bool CSyntaxAnalyzer::CheckInputSequence(const InputSequence & inputSeq)
{
	m_state = None;
	m_currentPos = 0;
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
	if (CheckWhileConstruction() || CheckIfConstruction() || CheckRead() || CheckPrint() || CheckAssignment() || CheckDeclare())
	{
		return CheckMainStruct();
	}
	else if (size_t nextExprLen = GetNextExpressionLength() != 0)
	{
		InputSequence seq = GetSequenceFromSequence(m_currentPos, nextExprLen, m_inputSeq);
		if (CheckArithmeticExpression(seq) /*BoolExpression(seq)*/)
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
		if (CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckBoolExpression, this, std::placeholders::_1)))
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
		if (CheckBracketsExpr(std::bind(&CSyntaxAnalyzer::CheckBoolExpression, this, std::placeholders::_1)))
		{
			return CheckProgramStruct();
		}
	}

	return false;
}

bool CSyntaxAnalyzer::CheckAssignment()
{
	if (MakeShiftIfNeeded(ID))
	{
		if (MakeShiftIfNeeded(ASSIGMENT))
		{
			auto assigmentSeq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(), m_inputSeq);
			if (CheckData(assigmentSeq))
			{
				m_currentPos += assigmentSeq.size();
				return MakeShiftIfNeeded(LINE_END);
			}
		}
		m_state = Error;
	}

	return false;
}

bool CSyntaxAnalyzer::CheckBoolExpression(const InputSequence &n)
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

//TODO выпилить бул и int они будут в таблице
bool CSyntaxAnalyzer::CheckData(const InputSequence & seq)
{
	return /*BoolExpression(seq) ||*/ CheckArithmeticExpression(seq) || CheckOneTokenExpr(TYPES_MAP.at(Types::STRING), seq) || CheckOneTokenExpr(TYPES_MAP.at(Types::CHAR), seq) || CheckOneTokenExpr(ID, seq) || CheckOneTokenExpr(TYPES_MAP.at(Types::INT), seq);
}

bool CSyntaxAnalyzer::CheckBracketsExpr(const CheckSequenceFunc & insideBracketsExpr)
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

bool CSyntaxAnalyzer::CheckDeclare()
{
	auto found = TYPES_MAP.end();

	if (m_currentPos < m_inputSeq.size())
	{
		found = std::find_if(TYPES_MAP.begin(), TYPES_MAP.end(), [=](const auto &typePair) { return typePair.second == m_inputSeq[m_currentPos]; });
	}

	if(found != TYPES_MAP.end() && MakeShiftIfNeeded(found->second))
	{
		if (MakeShiftIfNeeded(ID))
		{
			if (MakeShiftIfNeeded(ASSIGMENT))
			{
				auto seq = GetSequenceFromSequence(m_currentPos, GetNextExpressionLength(), m_inputSeq);
				if (CheckOneTokenExpr(ID, seq) || (m_checkTypesMap.at(found->first))(seq))
				{
					m_currentPos += seq.size();
				}
				else
				{
					m_state = Error;
				}
			}
			return MakeShiftIfNeeded(LINE_END);
		}
		m_state = Error;
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

bool CSyntaxAnalyzer::CheckOneTokenExpr(const std::string & token, const InputSequence & seq)
{
	if (seq.size() == 1)
	{
		return token == seq.front();
	}
	return false;
}

bool CSyntaxAnalyzer::CheckTypeAssigmentCorrectness(Types type)
{
	return false;
}

bool CSyntaxAnalyzer::CheckString(const InputSequence & seq)
{
	return CheckOneTokenExpr(TYPES_MAP.at(STRING), seq);
}

bool CSyntaxAnalyzer::CheckChar(const InputSequence & seq)
{
	return CheckOneTokenExpr(TYPES_MAP.at(CHAR), seq);
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
