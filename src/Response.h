#pragma once
#include <string>
#include <vector>
#include "element.h"
using namespace std;

class Response
{
public:
	Response(void);
	~Response(void);
	bool succeed;
	string info;
	vector < vector <element> > result;
	inline Response() { succeed=true; }
	inline Response( string data ) { succeed=false ; info=data; }
	inline Response( vector < vector <element> > result ):result(result),succeed(true)){}

	Response CreateIndex(const string indexName,const string &tableName , const string &itemName);
	Response DropIndex(const string indexName);
	Response CreateTable(const string &tableName,vector <item> data,int pk);
	Response DropTable(const string &tableName);
	Response Select(const string &tableName, const Fitter &fitter);
	Response Delete(const string &tableName, const Fitter &fitter);
	Response Insert(const string tableName, const vector<element> entry);
};



