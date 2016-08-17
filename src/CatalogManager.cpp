#include "CatalogManager.h"
#include "element.h"
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;


CatalogManager::CatalogManager(void)
{
	InitializeTable();
	InitializeIndex();
}


CatalogManager::~CatalogManager(void)
{
	writeback();
}

bool CatalogManager::IsTableExist(string tablename)
{
	return alltable.count(tablename);
}

bool CatalogManager::IsIndexExist(string indexname)
{
	return allindex.count(indexname);
}

bool CatalogManager::IsIndexRight(string tablename,string attrname)
{
	if(alltable.count(tablename))
	{
		vector<attr>::iterator it;
		for(it=alltable[tablename].allattr.begin();it!=alltable[tablename].allattr.end();++it)
		{
			if(attrname==(it->attrname))
				return 1;
		}
		cout<<"attrname is not exist"<<endl;
		return 0;
	}
	else
	{
		cout << "table is not exist"<<endl;
		return 0;
	}
}

bool CatalogManager::IsValueValid(string tablename, string value,int attrplace)
{
	int type=alltable[tablename].allattr.at(attrplace).attrtype;
	stringstream str;
	str << value;
	if(type<=255)
	{
		int len=value.length();
		if(value[0]=='\''&&value[len-1]=='\''&&(len-2)<=type)
			return 1;
		else
		{
			cout << "char values is not valid" <<endl;
			return 0;
		}
	}
	else if(type==256)
	{
		if(!(value[0]>='0'&&value[0]<='9'))
		{
			cout<< "int values is not valid" <<endl;
			return 0;
		}
		int intvalue;
		string useless;
		str >> intvalue;
		str >> useless;
		if(useless!="")
		{
			cout<< "int values is not valid" <<endl;
			return 0;
		}
		else
			return 1;
	}
	else if(type==257)
	{
		if(!(value[0]>='0'&&value[0]<='9'))
		{
			cout<< "float values is not valid" <<endl;
			return 0;
		}
		float floatvalue;
		string useless;
		str >> floatvalue;
		str >> useless;
		if(useless!="")
		{
			cout<< "float values is not valid" <<endl;
			return 0;
		}
		else
			return 1;
	}
	cout << "error in database"<<endl;
	return 0;
}

int CatalogManager::getattrpalce(string tablename, string attrname)
{
	vector<attr>::iterator it;
	int i=-1;
	for(it=alltable[tablename].allattr.begin();it!=alltable[tablename].allattr.end();++it)
	{
		i++;
		if(attrname==it->attrname)
			return i;
	}
	return -1;
}

void CatalogManager::addtable(string tablename,const Table& info)
{
	alltable[tablename]=info;
	tablenum++;
	return;
}

void CatalogManager::deltable(string tablename)
{
	if(alltable[tablename].indexnum>0)
	{
		string indexname;
		map<string,string>::iterator it;
		for(it=alltable[tablename].allindex.begin();it!=alltable[tablename].allindex.end();++it)
		{
			indexname=it->first;
			allindex.erase(indexname);	//delete index in catalog
			indexnum--;
		}
	}
	alltable.erase(tablename);
	tablenum--;
	return;
}

void CatalogManager::delindex(string indexname)
{
	string tablename=allindex[indexname].tablename;
	alltable[tablename].allindex.erase(indexname);
	alltable[tablename].indexnum--;
	allindex.erase(indexname);
	indexnum--;
	return;
}

void CatalogManager::addindex(string indexname,string attrname,string tablename)
{
	allindex[indexname].attrname=attrname;
	allindex[indexname].tablename=tablename;
	indexnum++;
	alltable[tablename].allindex[indexname]=attrname;
	alltable[tablename].indexnum++;
	return;
}

element CatalogManager::changevalue(string tablename,string value,int attrplace)
{
	int type=alltable[tablename].allattr.at(attrplace).attrtype;
	if(type<=255)
	{
		int len=value.length();
		element truevalue(value.substr(1,(len-1)));
		return truevalue;
	}
	else
	{
		element truevalue(value);	//关于float要不要做整数转换
		return truevalue;
	}
}

void CatalogManager::insert(string tablename)
{
	//table中要不要有recordnum
}

void CatalogManager::InitializeTable()
{
	string filename = "table.catalog";
	fstream fin((filename.c_str()),ios::in);
	fin >> tablenum;
	int i,j;
	string tablename;
	int attrnum;
	for(i=0;i<tablenum;i++)
	{
		fin >> tablename;
		fin >> attrnum;
		attr oneattr;
		vector<attr> attributes;
		for(j=0;j<attrnum;j++)
		{
			fin >> oneattr.attrname;
			fin >> oneattr.attrtype;
			fin >> oneattr.unique;
			attributes.push_back(oneattr);
		}
		string primary;
		fin >> primary;
		int numofindex;
		fin >> numofindex;
		string oneindexname;
		string oneindex;
		map<string,string> indexes;
		for(j=0;j<numofindex;j++)
		{
			fin >> oneindexname;
			fin >> oneindex;
			indexes[oneindexname]=oneindex;
		}
		Table onetable;
		onetable.allattr=attributes;
		onetable.allindex=indexes;
		onetable.attributenum=attrnum;
		onetable.indexnum=numofindex;
		onetable.primarykey=primary;
		alltable[tablename]=onetable;
	}
	fin.close();
}

void CatalogManager::InitializeIndex()
{
	string filename = "index.catalog";
	fstream fin((filename.c_str()),ios::in);
	fin >> indexnum;
	int i;
	string indexname;
	Index oneindex;
	for(i=0;i<indexnum;i++)
	{
		fin >> indexname;
		fin >> oneindex.attrname;
		fin >> oneindex.tablename;
		allindex[indexname]=oneindex;
	}
	fin.close();
}

void CatalogManager::writeback()
{
	string filename="table.catalog";
	fstream fout((filename.c_str()),ios::out);
	fout << tablenum <<endl;	//tablenum
	map<string,Table>::iterator it;
	for(it=alltable.begin();it!=alltable.end();++it)
	{
		fout << it->first <<endl;	//tablename
		fout << it->second.attributenum <<endl;	//attributenum
		vector<attr> attributes=it->second.allattr;
		vector<attr>::iterator itv;
		for(itv=attributes.begin();itv!=attributes.end();++itv)
		{
			fout << itv->attrname <<" ";
			fout << itv->attrtype <<" ";
			fout << itv->unique << endl;
		}
		fout << it->second.primarykey <<endl;	//primary key
		fout << it->second.indexnum <<endl;	//indexnum
		map<string,string>::iterator itm;
		for(itm=it->second.allindex.begin();itm!=it->second.allindex.end();++itm)
		{
			fout << itm->first<<" ";	//indexname
			fout << itm->second << endl;	//attrname
		}
	}
	fout.close();
	
	filename="index.catalog";
	fstream fout2((filename.c_str()),ios::out);
	fout2 << indexnum << endl;
	map<string,Index>::iterator iti;
	for(iti=allindex.begin();iti!=allindex.end();iti++)
	{
		fout2 << iti->first <<" ";
		fout2 << iti->second.attrname <<" ";
		fout2 << iti->second.tablename <<endl;
	}
	fout2.close();
}