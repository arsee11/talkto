#include <iostream>
#include <string>

#include <net/tcpsock.h>
#include <mvc/jpack.h>
#include <mvc/mvcrequester.h>

#include "console_view.h"

using namespace std;
using namespace arsee;

#if defined(_MSC_VER)
#pragma comment(lib, "libmynetd")
//#pragma comment(lib, "mvclibd")
#pragma comment(lib, "jsonlibd")
#endif

void TranMsg(JTcpRequester& rqt, const string& from, const string& to, const string &msg)
{
	rqt.source("msg");
	rqt.action("tran_msg");
	rqt.param("from", from);
	rqt.param("to", to);
	rqt.param("msg", msg);

	try{
		cout<<rqt.Request(10)<<endl;
	}
	catch (rqtexcpt &e)
	{
		cout << e.what() << endl;
	}
}

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
	
	MsgView view("console");
	ConsoleRspto rsp(&view);

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
			rqt.source(view.name());
			rqt.action(action);
			rqt.param("id", id);
			rqt.param("name", name);
			try{
				rqt.Request(rsp, 5);
				rsp.Update();
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
			rqt.source(view.name());
			rqt.action(action);
			rqt.param("id", id);
			rqt.param("key", key);
			
			try{
				rqt.Request(rsp, 10);
				rsp.Update();
			}
			catch(rqtexcpt &e)
			{
				cout<<e.what()<<endl;
			}
		}
		else if(action == "tran_msg")
		{
			string from, to, msg;
			cout << "from:";
			cin >> from;
			cout << "to:";
			cin >> to;
			cout << "msg:";
			cin >> msg;
			TranMsg(rqt, from, to, msg);
		}
	}

	SockUninit();
	return 0;
}
