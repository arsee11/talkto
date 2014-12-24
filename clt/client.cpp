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
	TcpSock::rpeer_ptr_t peer;
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
		peer = TcpSock::CreateClient(string(ip), port);
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
			Jpack pck("client", action);
			pck.Param("id", id);
			pck.Param("name", name);
		//	peer->Write(pck);
			char buf[1024] = { 0 };
			int len = peer->Read(buf, 1024);
			cout << "recv("<<len<<"):" << buf+16 << endl;
			
		}
		else if(action == "login")
		{
			Jpack::stream_t id, name;
			cout<<"id:";
			cin>>id;
			cout << "name:";
			cin>>name;
			JTcpRequester rqt(action);
			rqt.Open("115.29.178.221", 11111);
			rqt.Param("id", id);
			rqt.Param("name", name);
			string msg;
			try{
				rqt.Request(msg);
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
	peer->Close();
	SockUninit();
	return 0;
}
