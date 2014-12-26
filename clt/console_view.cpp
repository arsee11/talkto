//console_view.cpp

#include "console_view.h"
#include <iostream>

template<>
const string ConsoleView::base_t::_p1 = "msg";

int ConsoleView::Execute(const string &msg)
{
	cout << "Message:" << msg << endl;

	return 0;
}

template<>
const string MemberView::base_t::_p1 = "id";

template<>
const string MemberView::base_t::_p2 = "name";

template<>
const string MemberView::base_t::_p3 = "key";

int MemberView::Execute(size_t id, const string &name, const string &key)
{
	cout << id << endl;
	cout << "----------------------------" << endl;
	cout << "|" << name << "\t|" << key << "\t|" << endl;

	return 0;
}