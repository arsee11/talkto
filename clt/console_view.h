//console_view.h

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CONSOLE_VIEW_H
#define CONSOLE_VIEW_H

#include "mvc/mvcview.h"
#include "mvc/mvcresponseto.h"

#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

#include <string>

using namespace arsee;
using namespace std;

class ConsoleView :
	public MVCView<1, ConsoleView, string>
{
public:
	ConsoleView(const char* name)
		:_name(name)
	{}


	string name(){ return _name; }

	int Execute(const string &msg);

private:
	string _name;
};

typedef Responseto<ConsoleView, Jpack> ConsoleRspto;


//////////////////////////////////////////////////////////
class MemberView :
	public MVCView<3, ConsoleView, size_t, string, string>
{
public:
	string name(){ return "memberlistview"; }

	int Execute(size_t id, const string &name, const string &key);

};

typedef Responseto<MemberView, Jpack> MemberlistRspto;

#endif/*CONSOLE_VIEW_H*/