#include <iostream>
#include <string>

#include <net/tcpsock.h>
#include <mvc/jpack.h>
#include <mvc/mvcrequester.h>

using namespace std;
using namespace arsee;

#if defined(_MSC_VER)
#pragma comment(lib, "libmynetd")
//#pragma comment(lib, "mvclibd")
#pragma comment(lib, "jsonlibd")
#endif

int main(int args, char **argv)
{
	string ip = "115.29.178.221";
	unsigned short port = 11111;
	JTcpRequester rqt;
	if(args == 3)
	{
		ip = argv[1];
		port = atoi(argv[2]);
	}
	if(args == 2)
	{
		port = atoi(argv[1]);
	}
	
	try{
		SockInit();		
		rqt.Open("115.29.178.221", 11111);
	}
	catch(sockexcpt &e)
	{
		cout<<e.what()<<endl;
		return 1;
	}
	
	string action;
	
	while(action != "quit")
	{
		cout << "input action:";
		cin >> action;
		if(action == "registry")
		{
			Jpack::stream_t id, name;
			cout<<"id:";
			cin>>id;
			cout << "name:";
			cin>>name;
			rqt.Action(action);
			rqt.Param("id", id);
			rqt.Param("name", name);
			try{
				string msg = rqt.Request(5);
				cout << msg << endl;
			}
			catch (rqtexcpt &e)
			{
				cout << e.what() << endl;
			}
		
			
		}
		else if(action == "login")
		{
			Jpack::stream_t id, key;
			cout<<"id:";
			cin>>id;
			cout << "key:";
			cin>>key;
			rqt.Action(action);
			rqt.Param("id", id);
			rqt.Param("key", key);
			
			try{
				string msg = rqt.Request();
				cout<<msg<<endl;
			}
			catch(rqtexcpt &e)
			{
				cout<<e.what()<<endl;
			}
		}
		else if(action == "trans_msg")
		{
		}
	}

	SockUninit();
	return 0;
}
