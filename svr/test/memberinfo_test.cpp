//addmember_test.cpp

#include "../member_logics.h"
#include <iostream>
#pragma comment(lib, "d:\\mylib\\mylib\\lib\\mvclibd")
#pragma comment(lib, "E:\\lib\\odb\\libodb-2.3.0\\lib\\odb-d")
#pragma comment(lib, "E:\\lib\\odb\\libodb-mysql-2.3.0\\lib\\odb-mysql-d")


//g++ -std=c++11 -lodb -lodb-mysql addmember_test.cpp models-odb.cxx

int main()
{
	member_list_obj_t ms;
	member_ptr_t m(new Member("arsee01"));
	m->id(1);
	ms.ref().push_back(m);
	friends_obj_t fs;
	objects_t::Init(&ms, &fs);
	MemberInfo t;
	member_list_obj_t obj;
	//in list
	MemberInfo::response_t* rsp = t.Execute(nullptr, &obj, 1);
	Jpack* pck = rsp->Reply();
	cout << pck->status() << endl;
	cout << pck->action() << endl;
	cout << pck->target() << endl;

	//in persistent
	rsp = t.Execute(nullptr, &obj, 2);
	pck = rsp->Reply();
	cout << pck->status() << endl;
	cout << pck->action() << endl;
	cout << pck->target() << endl;

	//not exist
	rsp = t.Execute(nullptr, &obj, 100);

	return 0;
}