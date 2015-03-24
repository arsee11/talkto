//acceptfriendaction_test.cpp

#include "../member_logics.h"
#include <iostream>
#pragma comment(lib, "d:\\mylib\\mylib\\lib\\mvclibd")
#pragma comment(lib, "E:\\lib\\odb\\libodb-2.3.0\\lib\\odb-d")
#pragma comment(lib, "E:\\lib\\odb\\libodb-mysql-2.3.0\\lib\\odb-mysql-d")


//g++ -std=c++11 -lodb -lodb-mysql acceptfriendaction_test.cpp models-odb.cxx member_logics.cpp

int main()
{
	member_list_obj_t ms;
	friends_obj_t fs;
	objects_t::Init(&ms, &fs);
	AcceptFriendAction t;
	member_list_obj_t obj;
	Receiver rev = { 0, "192.168.1.104", 1122 };

	AcceptFriendAction::response_t* rsp = t.Execute(&rev, &fs, 3, 1, true, "hello");
	Jpack* pck = rsp->Reply();
	cout << pck->status() << endl;
	cout << pck->action() << endl;
	cout << pck->target() << endl;

	return 0;
}