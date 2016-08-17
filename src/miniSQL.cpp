#include "Interpreter.h"
#include "sqlAPI.h"
#include "CatalogManager.h"
#include <iostream>
#include <string>
using namespace std;

void welcom()
{
	cout<<"welcome to miniSQL"<<endl;
}

string Interpreter::command="";

sqlAPI api;
CatalogManager catalog;

int main()
{
	char input[1024];
	Interpreter interpret;
	welcom();
	while(1)
	{
		cout<<">>";
		gets_s(input);
		interpret.readcommand(input);
	}
}
