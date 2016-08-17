#pragma once
#include "Table.h"
#include "Index.h"
#include "Table.h"
#include "element.h"
#include <map>
#include <iostream>
using namespace std;

class CatalogManager
{
public:
	CatalogManager(void);
	~CatalogManager(void);
	bool IsTableExist(string tablename);
	bool IsIndexExist(string indexname);
	bool IsIndexRight(string tablename, string attrname);
	bool IsValueValid(string tablename, string value,int attrplace);

	void addtable(string tablename,const Table& info);
	void addindex(string indexname,string attrname,string tablename);
	void deltable(string tablenaem);
	void delindex(string indexname);
	void insert(string tablename);
	element changevalue(string tablename,string value,int attrplace);
	int getattrpalce(string tablename, string attrname);
	
	map<string,Table> alltable;
	map<string,Index> allindex;
	int tablenum;
	int indexnum;

private:
	void InitializeTable();
	void InitializeIndex();
	void writeback();
};

