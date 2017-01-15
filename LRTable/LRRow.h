#pragma once
#include <vector>
#include "Rule.h"

struct RowElement
{
	RowElement(const std::string & sym, size_t transition, bool shift)
		: inputSym(sym), transition(transition), shift(shift)
	{}

	RowElement(const std::string & sym, size_t transition, Rule rule, bool shift)
		: inputSym(sym), transition(transition), rollup(rule), shift(shift)
	{}
	std::string inputSym = "";
	int transition;
	boost::optional<Rule> rollup;
	bool shift;
};

typedef std::vector<std::vector<RowElement>> LRTable;