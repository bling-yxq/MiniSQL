#pragma once
#include <vector>
#include <iostream>
#include "element.h"
#define Le 0
#define Lt 1
#define Eq 2
#define Ne 3
#define Gt 4
#define Ge 5

using namespace std;

class sqlAPI
{
public:
	sqlAPI(void);
	~sqlAPI(void);
	void createtable(const vector<string>& arg);
	void createindex(const vector<string>& arg);
	void droptable(const vector<string>& arg);
	void dropindex(const vector<string>& arg);
	void insert(string tablename, const vector<element>& values);
	void showtable(const vector<string>& arg);	//返回一个vector存储结果
	void select(const vector<string>& arg);	//返回一个vector存储结果
	int deleteall(const vector<string>& arg);
	int deletetable(const vector<string>& arg);
};

