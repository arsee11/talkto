#if defined(_MSC_VER)
#pragma comment(lib, "../../bin/mvclibd")
#pragma comment(lib, "../../bin/jsonlibd")
#endif

#include "mvc/server.h"
#include "logics.h"
#include "mysession.h"
#include "net/acceptor.h"


typedef PreactorServer<tcp_preactor_t, 
	Acceptor<mysession_t> 
> svr_t;

int main(int argc, char **argv)
{
	member_list_obj_t mlobj;
	//msg_list_t msgl_obj;

	mlobj.Attach( "memberview" );
	
	SockInit();
	objects_t::Init(&mlobj);

	SockConfig conf = { 11111, 0, "", "" };
	
	svr_t svr(1024, conf);
	
	try{
		while (true)
			svr.Run();
	}
	catch (exception &e){
		cout << e.what() << endl;
	}

	SockUninit();
	return 0;

}

