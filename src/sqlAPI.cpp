#include "sqlAPI.h"
#include "attr.h"
#include "Table.h"
#include "CatalogManager.h"
#include "Condition.h"
#include "element.h"
#include "InputException.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

extern CatalogManager catalog;

sqlAPI::sqlAPI(void)
{
}


sqlAPI::~sqlAPI(void)
{
}

void sqlAPI::createtable(const vector<string>& arg)
{
	int size;
	string contain;
	size=arg.size();
	string tablename;
	string primary;
	string type;
	string temp;
	int attrtype;
	int attrnum;
	int unique;
	attr oneattr;
	vector<attr> attribute;
	string attrname;
	int i,keyflag;
	size=arg.size();
	tablename=arg.at(--size);
	primary=arg.at(--size);
	keyflag=-1;
	attrnum=0;
	for(i=0;i<size;attrnum++)	//get attribute and type
	{
		unique=0;
		attrname=arg.at(i++);
		if(attrname==primary)
		{
			keyflag=attrnum;
			unique=1;
		}
		type=arg.at(i++);
		if(type=="int")
			attrtype=256;
		else if(type=="float")
			attrtype=257;
		else if(type.substr(0,4)=="char")
		{
			if(type.length()<7)
			{
				InputException E(0);
				throw(E);
			}
			attrtype=atoi(type.substr(5,type.size()-1).c_str());
			if(attrtype==0 || attrtype>255)
			{
				InputException E(0);
				throw(E);
			}
		}
		else
		{
			InputException E(0);
			throw(E);
		}
		temp=arg.at(i);
		if(temp=="unique")
		{
			unique=1;
			i++;
		}
		oneattr.attrname=attrname;
		oneattr.attrtype=attrtype;
		oneattr.unique=unique;
		attribute.push_back(oneattr);
	}
	if(keyflag==-1)
	{
		InputException E(1);
		throw(E);
	}
	Table tableinfo;
	tableinfo.allattr=attribute;
	tableinfo.primarykey=primary;
	tableinfo.attributenum=attrnum;
	tableinfo.indexnum=0;
	catalog.addtable(tablename,tableinfo);
	//for recordmanager to createtable
}

void sqlAPI::createindex(const vector<string>& arg)
{
	int size;
	size=arg.size();
	if(catalog.IsIndexRight(arg.at(size-2),arg.at(size-3)))
	{
		string indexname=arg.at(size-1);
		string tablename=arg.at(size-2);
		string attrname=arg.at(size-3);
		catalog.addindex(indexname,attrname,tablename);
		//for recordmanager to createindex
	}
	else
	{
		InputException E(2);
		throw(E);
	}
}

void sqlAPI::droptable(const vector<string>& arg)
{
	int size=arg.size();
	string tablename=arg.at(size-1);
	catalog.deltable(tablename);
	//for recordmanager to droptable
}

void sqlAPI::dropindex(const vector<string>& arg)
{
	string indexname=arg.at(arg.size()-1);
	catalog.delindex(indexname);
	//for recordermanager to dropindex 
}

void sqlAPI::insert(string tablename, const vector<element>& values)
{
	catalog.insert(tablename);
	//for recordmanager to insert
}

void sqlAPI::showtable(const vector<string>& arg)
{
	string tablename=arg.at(arg.size()-1);
	//for recordmanager to showtable
}

void sqlAPI::select(const vector<string>& arg)
{
	int size=arg.size();
	string tablename=arg.at(size-1);
	string attrname;
	string compare;
	string value;
	int attrplace;
	int compareint;
	int condnum=0;
	Condition onecondition;
	vector<Condition> conditions;
	for(int i=size-2;i>=0;)
	{
		attrname=arg.at(i--);
		compare=arg.at(i--);
		value=arg.at(i--);
		attrplace=catalog.getattrpalce(tablename,attrname);
		if(attrplace==-1)
		{
			InputException E(3);
			throw(E);
		}
		else
		{
			if(catalog.IsValueValid(tablename,value,attrplace))
			{
				element truevalue=catalog.changevalue(tablename,value,attrplace);
				if(compare=="<")
					compareint=Lt;
				else if(compare=="<=")
					compareint=Le;
				else if(compare=="=")
					compareint=Eq;
				else if(compare=="<>")
					compareint=Ne;
				else if(compare==">=")
					compareint=Ge;
				else if(compare==">")
					compareint=Gt;
				else
				{
					InputException E(4);
					throw(E);
				}
				onecondition.attrplace=attrplace;
				onecondition.op=compareint;
				onecondition.value=truevalue;	//value的类型是否需要确定
				conditions.push_back(onecondition);
				condnum++;
			}
			else
			{
				InputException E(5);
				throw(E);
			}
		}
	}
	//for recordmanager to select
}

int sqlAPI::deleteall(const vector<string>& arg)
{
	string tablename=arg.at(arg.size()-1);
	//for recordmanager to delete
	return 1;
}

int sqlAPI::deletetable(const vector<string>& arg)
{
	int size=arg.size();
	string tablename=arg.at(size-1);
	string attrname;
	string compare;
	string value;
	int attrplace;
	int compareint;
	int condnum=0;
	Condition onecondition;
	vector<Condition> conditions;
	for(int i=size-2;i>=0;)
	{
		attrname=arg.at(i--);
		compare=arg.at(i--);
		value=arg.at(i--);
		attrplace=catalog.getattrpalce(tablename,attrname);
		if(attrplace==-1)
		{
			InputException E(3);
			throw(E);
		}
		else
		{
			if(catalog.IsValueValid(tablename,value,attrplace))
			{
				element truevalue=catalog.changevalue(tablename,value,attrplace);
				if(compare=="<")
					compareint=Lt;
				else if(compare=="<=")
					compareint=Le;
				else if(compare=="=")
					compareint=Eq;
				else if(compare=="<>")
					compareint=Ne;
				else if(compare==">=")
					compareint=Ge;
				else if(compare==">")
					compareint=Gt;
				else
				{
					InputException E(4);
					throw(E);
				}
				onecondition.attrplace=attrplace;
				onecondition.op=compareint;
				onecondition.value=truevalue;	//value的类型是否需要确定
				conditions.push_back(onecondition);
				condnum++;
			}
			else
			{
				InputException E(5);
				throw(E);
			}
		}
	}
	//for recordmanager to select
}