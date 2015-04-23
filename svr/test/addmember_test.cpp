#include "../member_logics.h"
#include <iostream>
#pragma comment(lib, "d:\\mylib\\mylib\\lib\\mvclibd")
#pragma comment(lib, "E:\\lib\\odb\\libodb-2.3.0\\lib\\odb-d")
#pragma comment(lib, "E:\\lib\\odb\\libodb-mysql-2.3.0\\lib\\odb-mysql-d")


//g++ -g -std=c++11 -I../../../mylib addmember_test.cpp ../member_logics.cpp ../odb/models-odb.cxx ../logics.cpp -lodb -lodb-mysql
 
 int main()
 {
	AddMember add;
	member_list_obj_t obj;
	AddMember::response_t* rsp = add.Execute(nullptr, &obj, "arsee02", "123");
	Jpack* pck = rsp->Reply();
	cout<<pck->status()<<endl;
	cout<<pck->action()<<endl;
	cout<<pck->target()<<endl;
	
	return 0;
 }
