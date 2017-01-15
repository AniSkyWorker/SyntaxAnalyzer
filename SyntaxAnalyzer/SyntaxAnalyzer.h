#pragma once
#include <string>
#include <vector>
#include <functional>
#include "LL1Walker.h"
#include "Types.h"
#include "TableStorage.h"

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
	void CheckInputSequence(const InputSequence & inputSeq);

private:
	void CheckProgramStruct();
	void CheckMainStruct();

	bool CheckWhileConstruction();
	bool CheckIfConstruction();
	void CheckElIfConstruction();

	bool CheckAssignment();
	bool CheckBoolExpression(const InputSequence & seq);
	bool CheckArithmeticExpression(const InputSequence & seq);

	bool CheckPrint();
	bool CheckRead();

	bool CheckData(const InputSequence &);
	bool CheckString(const InputSequence & seq);
	bool CheckChar(const InputSequence & seq);

	void CheckBracketsExpr(const CheckSequenceFunc & insideBracketsExpr);

	bool CheckDeclare();

	bool MakeShiftIfNeeded(const std::string & checkStr, bool shift = true, bool exception = false);

	bool ChecIndexStruct(const InputSequence & seq, size_t start = 0);
	bool CheckIndex(const InputSequence & seq, size_t & start);
	bool CheckTypeWithIndecies(const std::string & type, const InputSequence & seq);

	size_t GetNextExpressionLength();

	LL1Walker m_LL1Walker;
	CTableStorage m_tableStorage;

	InputSequence m_inputSeq;
	size_t m_currentPos = 0;
	const std::map<Types, CheckSequenceFunc> m_checkTypesMap;
};

