//console_view.cpp

#include "console_view.h"
#include <iostream>

int RequestView::Execute(const string &msg)
{
	cout << msg << endl;

	return 0;
}


int MemberView::Execute(size_t id, const string &name, const string &key)
{
	cout << id << endl;
	cout << "----------------------------" << endl;
	cout << "|" << name << "\t|" << key << "\t|" << endl;

	return 0;
}


int MsgView::Execute(size_t from, const string &msg)
{
	cout << endl;
	cout << "Receive a message" << endl;
	cout << "---------------------------" << endl;
	cout << "from:" << from << endl;
	cout << "content:" << msg << endl;
	return 0;
}