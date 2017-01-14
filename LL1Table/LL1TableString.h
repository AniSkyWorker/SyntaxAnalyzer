#pragma once
#include <vector>

struct LL1TableString
{
	LL1TableString(const std::vector<std::string> & input, bool shift, int transition, bool stack, bool error, bool end);

	int transition;
	bool error;
	bool shift;
	bool end;
	bool stack;
	std::vector<std::string> input;
};

typedef std::vector<LL1TableString> LL1Table;