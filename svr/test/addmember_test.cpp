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
	string name = "arsee02";
	string pwd = "123";
	AddMember::response_t* rsp = add.Execute(nullptr, &obj, name, pwd); 
	Jpack* pck = rsp->Reply();
	cout<<"status:"<<pck->status()<<endl;
	cout<<"action:"<<pck->action()<<endl;
	cout<<"target:"<<pck->target()<<endl;
	cout<<"param:code:"<<pck->get_param(0, "code")<<endl;
	
	return 0;
 }
