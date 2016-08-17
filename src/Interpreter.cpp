#include "Interpreter.h"
#include "CatalogManager.h"
#include "sqlAPI.h"
#include "InputException.h"
#include "element.h"
#include <strstream>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

extern sqlAPI api;
extern CatalogManager catalog;

Interpreter::Interpreter(void)
{
}

Interpreter::~Interpreter(void)
{
}

void Interpreter::execfile(string filename)
{
	arg.clear();
	ifstream file(filename.c_str());
	string line;
	while(getline(file,line))
	{
		readcommand(line);
	}
	return;
}

void Interpreter::execute()
{
	cout<<op<<endl;
	int size;
	size=arg.size();
	cout<<"size "<<size<<endl;
	switch(op)
	{
	case 0:	//execfile
		{
			string filename=arg.at(arg.size()-1);
			execfile(filename);
			arg.clear();
			return;
		}
	case 1:	//create table
		if(!catalog.IsTableExist(arg.at(size-1)))
		{
			try
			{
				api.createtable(arg);
				cout<<"create table success"<<endl;
			}
			catch(InputException E)
			{
				switch(E.flag)
				{
				case 0: cout<<"attribute type is not correct"<<endl; break;
				case 1: cout<<"primary key is not an attribute"<<endl;break;
				}
			}
		}
		else
		{
			cout<<"table is exist, create table failed"<<endl;
		}
		arg.clear();
		return;
	case 2:	//create index
		if(!catalog.IsIndexExist(arg.at(size-1)))
		{
			try
			{
				api.createindex(arg);
				cout<<"create index success"<<endl;
			}
			catch(InputException E)
			{
				cout <<"index information is not valid"<<endl;
			}
		}
		else
		{
			cout<<"index is exist,create index failed"<<endl;
		}
		arg.clear();
		return;
	case 3:	//drop table
		if(catalog.IsTableExist(arg.at(size-1)))
		{
			try
			{
				api.droptable(arg);
				cout<<"drop table success"<<endl;
			}
			catch(InputException E)
			{
				cout <<"drop table failed"<<endl;
			}
		}
		else
		{
			cout<<"table is not exist, drop table failed"<<endl;
		}
		arg.clear();
		return;
	case 4:	//drop index
		if(catalog.IsIndexExist(arg.at(size-1)))
		{
			try
			{
				api.dropindex(arg);
				cout<<"drop index success"<<endl;
			}
			catch(InputException E)
			{
				cout <<"drop index failed"<<endl;
			}
		}
		else
		{
			cout<<"index is not exist, drop index failed"<<endl;
		}
		arg.clear();
		return;
	case 5:	//insert
		if(catalog.IsTableExist(arg.at(size-1)))
		{
			int size=arg.size();
			string tablename=arg.at(size-1);
			vector<element> values;
			string value;
			int i;
			for(i=0;i<size-1;i++)
			{
				value=arg.at(i);
				if(catalog.IsValueValid(tablename,value,i))	//test if values is right type
				{
					element truevalue=catalog.changevalue(tablename,value,i);
					values.push_back(truevalue);
				}
				else
					break;
			}
			if(i!=(size-1))
			{
				cout <<"value is not match with attribute type,insert failed"<<endl;
				arg.clear();
				return;
			}
			try
			{
				api.insert(tablename,values);
				cout<<"insert value success"<<endl;
			}
			catch(InputException E)
			{
				cout <<"insert value failed"<<endl;
			}
		}
		else
		{
			cout<<"table is not exist, insert value failed"<<endl;
		}
		arg.clear();
		return;
	case 6:	//showtable
		if(catalog.IsTableExist(arg.at(size-1)))
		{
			try
			{
				api.showtable(arg);
				cout<<"show table success"<<endl;
			}
			catch(InputException E)
			{
				cout<<"show table failed"<<endl;
			}
		}
		else
		{
			cout<<"table is not exist, show table failed"<<endl;
		}
		arg.clear();
		return;
	case 7:	//select
		if(catalog.IsTableExist(arg.at(size-1)))
		{
			try
			{
				api.select(arg);
				cout<<"select table success"<<endl;
			}
			catch(InputException E)
			{
				switch(E.flag)
				{
				case 3:
					cout<<"attribute is not exist"<<endl;break;
				case 4:
					cout<<"compare operation is not correct"<<endl;break;
				case 5:
					cout<<"compare value is not valid"<<endl;break;
				}
				cout<<"select table failed"<<endl;
			}
		}
		else
		{
			cout<<"table is not exist, select table failed"<<endl;
		}
		arg.clear();
		return;
	case 8:
		if(catalog.IsTableExist(arg.at(size-1)))
		{
			try
			{
				int delnum;
				delnum=api.deleteall(arg);
				cout<<"delete record success, delnum is: "<<delnum<<endl;
			}
			catch(InputException E)
			{
				cout<<"delete table record failed"<<endl;
			}
		}
		else
		{
			cout<<"table is not exist, delete table record failed"<<endl;		
		}
		arg.clear();
		return;	
	case 9:
		if(catalog.IsTableExist(arg.at(size-1)))
		{
			try
			{
				int delnum;
				api.deletetable(arg);
				cout<<"delete record success, delnum is: "<<delnum<<endl;
			}
			catch(InputException E)
			{
				switch(E.flag)
				{
				case 3:
					cout<<"attribute is not exist"<<endl;break;
				case 4:
					cout<<"compare operation is not correct"<<endl;break;
				case 5:
					cout<<"compare value is not valid"<<endl;break;
				}
				cout<<"delete table record failed"<<endl;
			}
		}
		else
		{
			cout<<"table is not exist, delete table record failed"<<endl;
		}
		arg.clear();
		return;
	case 10:
		cout<<"error input"<<endl;
	//	guide();
		arg.clear();
		break;
	default:
		cout<<"unknow state!"<<endl;
		return;
	}
	return;
}

void Interpreter::readcommand(string input)
{
	unsigned int i;
	int end;
	for(i=0;i<input.length();i++)
	{
		if(input[i]==';')
		{
			string readover=command;
			parse(readover);
			end=1;
			command="";
		}
		else
		{
			command=command+input[i];
			end=0;
		}
	}
	if(end==0)
		command=command+" ";
	else
		execute();
	return;
}

void Interpreter::parse(string input)
{
	op=UNKNOW;
	if(input=="quit")	//quit;
	{
		cout<<"bye-bye"<<endl;
		exit(0);
	}
	
	if (input=="")	//;
	{
		cout<<"no command before ;"<<endl;
		return;
	}
	
	strstream str;
	str<<input;
	string operation;
	string useless;
	str>>operation;
	if(operation=="execfile")//execfile filename;
	{
		string filename;
		str>>filename;
		op=EXEC;
		arg.push_back(filename);
		return;
	}
	if (operation=="create")
	{
		string type;
		str>>type;
		if (type=="table") 
		{
			string tablename;
			str>>tablename;	//tablename
			str>>useless;	//(
			if(useless!="(")
			{
				op=INPUTERR;
				return;
			}
			string attribute;
			int attributeNum=0;
			string temp;
			while(1)
			{
				str>>temp;
				
				if(temp=="primary")
				{
					if(attributeNum==0)
					{
						op=INPUTERR;
						return;
					}
					else
					{
						str>>useless;	//key
						if(useless!="key")
						{
							op=INPUTERR;
							return;
						}
						str>>useless;	//(
						if(useless!="(")
						{
							op=INPUTERR;
							return;
						}
						str>>attribute;	//primary key name
						arg.push_back(attribute);
						break;	//end of create table
					}
				}
				else
				{
					string attrtype;
					attribute=temp;
					arg.push_back(attribute);
					str>>attrtype;	//int\float\char(*)
					arg.push_back(attrtype);
					str>>temp;
					if(temp=="unique")
					{
						arg.push_back("unique");
						str>>useless;	//,
						if(useless!=",")
						{
							op=INPUTERR;
							return;
						}
					}
					else if(temp!=",")
					{
						op=INPUTERR;
						return;
					}
					attributeNum++;
				}
			}
			arg.push_back(tablename);
			op=CREATETAB;
			return;
		}
		else if(type=="index")
		{
			string indexname;
			str>>indexname;
			str>>useless;	//on
			if(useless!="on")
			{
				op=INPUTERR;
				return;
			}
			string tablename;
			str>>tablename;	//tablename
			str>>useless;	//(
			if(useless!="(")
			{
				op=INPUTERR;
				return;
			}
			string attribute;
			str>>attribute;
			str>>useless;	//)
			if(useless!=")")
			{
				op=INPUTERR;
				return;
			}
			arg.push_back(attribute);
			arg.push_back(tablename);
			arg.push_back(indexname);
			op=CREATEINDEX;
			return;
		}
		else
		{
			op=INPUTERR;
			return;
		}
	}
	else if(operation=="drop")
	{
		string type;
		str>>type;
		if(type=="table")
		{
			string tablename;
			str>>tablename;
			arg.push_back(tablename);
			op=DROPTAB;
			return;
		}
		else if(type=="index")
		{
			string indexname;
			str>>indexname;
			arg.push_back(indexname);
			op=DROPINDEX;
			return;
		}
		else
		{
			op=INPUTERR;
			return;
		}
	}
	else if(operation=="insert")
	{
		string tablename;
		str>>useless;	//into
		if(useless!="into")
		{
			op=INPUTERR;
			return;
		}
		str>>tablename;
		str>>useless;	//values
		if(useless!="values")
		{
			op=INPUTERR;
			return;
		}
		str>>useless;//(
		if(useless!="(")
		{
			op=INPUTERR;
			return;
		}
		while(1)
		{
			string values;
			str>>values;
			arg.push_back(values);
			str>>useless;
			if(useless!=","&&useless!=")")
			{
				op=INPUTERR;
				return;
			}
			if(useless==")")
				break;
		}
		op=INSERT;
		arg.push_back(tablename);
	}
	else if(operation=="select")
	{
		string tablename;
		str>>useless;	//*
		if(useless!="*")
		{
			op=INPUTERR;
			return;
		}
		str>>useless;	//from
		if(useless!="from")
		{
			op=INPUTERR;
			return;
		}
		str>>tablename;
		int conditionnum=0;
		while(str>>useless)	//where\and
		{
			string attribute;
			string compare;
			string value;
			str>>attribute;
			str>>compare;
			str>>value;
			arg.push_back(value);
			arg.push_back(compare);
			arg.push_back(attribute);
			conditionnum++;
		}
		if(conditionnum==0)
			op=SHOWTAB;
		else
			op=SELECT;
		arg.push_back(tablename);
		return;
	}
	else if(operation=="delete")
	{
		string tablename;
		str>>useless;	//from
		if(useless!="from")
		{
			op=INPUTERR;
			return;
		}
		str>>tablename;
		int conditionnum=0;
		while(str>>useless)	//where\and
		{
			string attribute;
			string compare;
			string value;
			str>>attribute;
			str>>compare;
			str>>value;
			arg.push_back(value);
			arg.push_back(compare);
			arg.push_back(attribute);
			conditionnum++;
		}
		if(conditionnum==0)
			op=DELETEALL;
		else
			op=DELETE;
		arg.push_back(tablename);
		return;
	}
	else
	{
		cout<<"no such command ";
		op=INPUTERR;
		return;
	}
}
