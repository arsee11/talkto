//console_view.cpp

#include "console_view.h"
#include <iostream>

int ConsoleView::Execute(const string &msg)
{
	cout << "Message:" << msg << endl;

	return 0;
}


int MemberView::Execute(size_t id, const string &name, const string &key)
{
	cout << id << endl;
	cout << "----------------------------" << endl;
	cout << "|" << name << "\t|" << key << "\t|" << endl;

	return 0;
}
