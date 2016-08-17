#pragma once
#include "element.h"
#include <string>
#include <iostream>
using namespace std;

class Condition
{
public:
	Condition(void);
	~Condition(void);

	int attrplace;
	int op;
	element value;
};

