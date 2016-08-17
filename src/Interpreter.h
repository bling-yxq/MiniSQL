#pragma once
#define EXEC 0
#define CREATETAB 1
#define CREATEINDEX 2
#define DROPTAB 3
#define DROPINDEX 4
#define INSERT 5
#define SHOWTAB 6
#define SELECT 7
#define DELETEALL 8
#define DELETE 9
#define INPUTERR 10
#define UNKNOW 11
#include <vector>
#include <iostream>
using namespace std;

class Interpreter
{
public:
	static string command;
	int op;
	vector<string> arg;
	Interpreter(void);
	~Interpreter(void);
	
	void readcommand(string input);
	void parse(string input);
	void execute();
	void execfile(string filename);
};

