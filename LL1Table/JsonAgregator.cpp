#include "stdafx.h"
#include "JsonAgregator.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;

void CJsonAgregator::WriteLL1TableToFile(const LL1Table & table, const std::string & filename)
{
	ofstream output(filename);
	json jsonTable;
	for (size_t i = 0; i < table.size(); i++)
	{
		jsonTable[i]["input_sequence"] = table[i].input;
		jsonTable[i]["shift"] = table[i].shift;
		jsonTable[i]["transition"] = table[i].transition;
		jsonTable[i]["stack"] = table[i].stack;
		jsonTable[i]["error"] = table[i].error;
		jsonTable[i]["end"] = table[i].end;
	}
	output << jsonTable;
	output.close();
}

LL1Table CJsonAgregator::ReadLL1TableFromFile(const std::string & filename)
{
	ifstream input(filename);
	json jsonTable(input);
	LL1Table table;
	for (auto row : jsonTable)
	{
		table.emplace_back(LL1TableString(row["input_sequence"], row["shift"], row["transition"], row["stack"], row["error"], row["end"]));
	}
	input.close();
	return table;
}

