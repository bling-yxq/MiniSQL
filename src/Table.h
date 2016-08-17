#pragma once
#include <vector>
#include <map>
#include "attr.h"
#include <iostream>
#include <string>
using namespace std;

class Table
{
public:
	Table(void);
	~Table(void);
	vector<attr> allattr;
	string primarykey;
	int attributenum;
	int indexnum;
	map<string,string> allindex;
	//int recordnum;
};

