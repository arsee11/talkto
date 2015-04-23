//transmsg_test.cpp

#include "../msg_logics.h"
#include <iostream>


//g++ -g -std=c++11 -I../../../mylib transmsg_test.cpp ../member_logics.cpp ../odb/models-odb.cxx ../logics.cpp -lodb -lodb-mysql
 
 int main()
 {
	TransMstTo t;
	friends_obj_t obj;
	TransMstTo::response_t* rsp = t.Execute(nullptr, &obj, 1, 2, "hello");
	Jpack* pck = rsp->Reply();
	cout<<pck->status()<<endl;
	cout<<pck->action()<<endl;
	cout<<pck->target()<<endl;
	
	return 0;
 }
