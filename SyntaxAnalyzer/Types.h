#pragma once
#include <map>
#include <string>

enum Types
{
	BOOL,
	INT,
	FLOAT,
	CHAR,
	STRING
};


const std::map<Types, std::string> TYPES_MAP =
{
	{ Types::BOOL, "booltype" },
	{ Types::INT, "inttype" },
	{ Types::CHAR, "chartype" },
	{ Types::STRING, "stringtype" },
	{ Types::FLOAT, "floattype" }
};