#pragma once
#include "TableType.h"
#include "LL1TableString.h"
#include "LRRow.h"

class CTableStorage
{
public:
	CTableStorage();
	~CTableStorage();

	const LL1Table & GetLL1Table(TableType type);
	const LRTable & GetLRTable(TableType type);
private:
	void LoadTables();

	std::map<TableType, LL1Table> m_LL1Tables;
	std::map<TableType, LRTable> m_LRTables;
};

