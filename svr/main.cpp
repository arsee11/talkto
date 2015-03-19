#if defined(_MSC_VER)
#pragma comment(lib, "../../bin/mvclibd")
#pragma comment(lib, "../../bin/jsonlibd")
#endif

#include "mvc/server.h"
#include "logics.h"
#include "myconnection.h"
#ifndef ACCEPTOR_H
#include "net/acceptor.h"
#endif


typedef PreactorServer<tcp_preactor_t, 
	net::Acceptor<myconnection_t> 
> svr_t;

int main(int argc, char **argv)
{
	member_list_obj_t mlobj;
	//msg_list_t msgl_obj;

	net::SockInit();
	objects_t::Init(&mlobj);

	net::SockConfig conf = { 11111, 0, "", "" };
	
	svr_t svr(1024, conf);
	
	try{
		while (true)
			svr.Run();
	}
	catch (std::exception &e){
		cout << e.what() << endl;
	}

	net::SockUninit();
	return 0;

}

