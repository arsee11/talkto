//makefriendaction_test.cpp

#include "../member_logics.h"
#include <iostream>
#pragma comment(lib, "d:\\mylib\\mylib\\lib\\mvclibd")
#pragma comment(lib, "E:\\lib\\odb\\libodb-2.3.0\\lib\\odb-d")
#pragma comment(lib, "E:\\lib\\odb\\libodb-mysql-2.3.0\\lib\\odb-mysql-d")


//g++ -g -std=c++11 -I../../../mylib makefriendaction_test.cpp ../member_logics.cpp ../odb/models-odb.cxx ../logics.cpp -lodb -lodb-mysql

int main()
{
	member_list_obj_t ms;
	friends_obj_t fs;
	objects_t::Init(&ms, &fs);
	MakeFriendAction t;
	member_list_obj_t obj;
	Receiver rev = { 0, "192.168.1.104", 1122 };

	MakeFriendAction::response_t* rsp = t.Execute(&rev, &fs, 3, 1);
	Jpack* pck = rsp->Reply();
	cout << pck->status() << endl;
	cout << pck->action() << endl;
	cout << pck->target() << endl;

	return 0;
}
