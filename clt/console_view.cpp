//console_view.cpp

#include "console_view.h"
#include <iostream>

int RequestView::Execute(int code)
{
	if(code == 0)
		cout << "quest ok!" << endl;
	if(code == 1)
		cout<<"quest failed"<<endl;

	return 0;
}


int MemberView::Execute(size_t id, const string &name)
{
	cout << id << endl;
	cout << "----------------------------" << endl;
	cout << "|" << name << "\t|" << endl;

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