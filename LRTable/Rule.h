#pragma once
#include <string>
#include <boost/optional.hpp>

struct Rule
{
	Rule(const std::string & output, unsigned size)
		:outputSym(output), size(size)
	{}

	std::string outputSym;
	unsigned size;
};