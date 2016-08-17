#pragma once
#include <string>
#include <iostream>
using namespace std;

class attr
{
public:
	attr(void);
	~attr(void);
	string attrname;
	int attrtype;	//char(0~255) int256 float257
	int unique;
};

