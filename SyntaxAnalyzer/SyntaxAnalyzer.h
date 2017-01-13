#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../LL1Table/LL1Walker.h"
#include "Types.h"

typedef std::vector<std::string> InputSequence;
typedef std::function<bool(const InputSequence &)> CheckSequenceFunc;

enum State
{
	Error,
	None
};

class CSyntaxAnalyzer
{
public:
	CSyntaxAnalyzer();
	bool CheckInputSequence(const InputSequence & inputSeq);

private:
	bool CheckProgramStruct();
	bool CheckMainStruct();

	bool CheckWhileConstruction();
	bool CheckIfConstruction();

	bool CheckAssignment();
	bool CheckBoolExpression(const InputSequence & seq);
	bool CheckArithmeticExpression(const InputSequence & seq);

	bool CheckPrint();
	bool CheckRead();
	bool CheckData(const InputSequence &);

	bool CheckBracketsExpr(const CheckSequenceFunc & insideBracketsExpr);

	bool CheckDeclare();

	bool MakeShiftIfNeeded(const std::string & checkStr);
	bool CheckOneTokenExpr(const std::string & token, const InputSequence & seq);
	
	bool CheckTypeAssigmentCorrectness(Types type);

	bool CheckString(const InputSequence & seq);
	bool CheckChar(const InputSequence & seq);

	size_t GetNextExpressionLength();

	LL1Walker llWalker;
	InputSequence m_inputSeq;
	size_t m_currentPos = 0;
	State m_state = None;
	const std::map<Types, CheckSequenceFunc> m_checkTypesMap;
};

