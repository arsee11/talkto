//console_view.h

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CONSOLE_VIEW_H
#define CONSOLE_VIEW_H

#include "mvc/mvcview.h"
#include "mvc/mvclresponse.h"

#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

#include <string>

using namespace arsee;
using namespace std;

class RequestView :
	public MVCView<1, RequestView, string>
{
public:
	RequestView(const char* name)
		:_name(name)
	{}


	string name(){ return _name; }

	int Execute(const string &msg);
	static const string p1(){return "msg";}

private:
	string _name;
};

//////////////////////////////////////////////////////////
class MemberView :
	public MVCView<3, MemberView, size_t, string, string>
{
public:
	string name(){ return "memberlistview"; }

	int Execute(size_t id, const string &name, const string &key);
	static const string p1(){ return "id";}
	static const string p2(){ return "name";}
	static const string p3(){ return "key";}
};

//////////////////////////////////////////////////////////
class MsgView :
	public MVCView<2, MsgView, size_t, string>
{
public:
	string name(){ return "msgview"; }

	int Execute(size_t from, const string &msg);
	static const string p1(){ return "from"; }
	static const string p2(){ return "msg"; }
};



#endif/*CONSOLE_VIEW_H*/
