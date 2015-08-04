//MyConnection.h


#include "controls.h"
#include "mvc/utility.h"

#ifndef SESSIOIN_H
#include "net/session.h"
#endif

#ifndef PREACTOR_H
#include "net/preactor.h"
#endif

#ifndef FD_H
#include "net/fd.h"
#endif

#ifndef SELECTOR_H
#include "net/selector.h"
#endif

#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

#ifndef CONTROLS_H
#include "controls.h"
#endif

#ifndef OBJECTS_H
#include "objects.h"
#endif

#ifndef MVC_REQUEST_CONTEXT_H 
#include "mvc/mvc_request_context.h"
#endif

#ifndef TCPSOCK_H 
#include "net/tcpsock.h"
#endif

#include <vector>
#include <fstream>

using namespace std;
using namespace arsee;

enum{ MAX_BUF_LEN=1500};

static ofstream fs("log.txt");

typedef net::Preactor<net::FdHolder, true, net::Epoll> tcp_preactor_t;

template<class Pack, class ObjCollection, class... Dispatchers>
class MyConnection :
	public net::Session<MAX_BUF_LEN, tcp_preactor_t, MyConnection<Pack, ObjCollection, Dispatchers...> >
{
	typedef Pack pack_t;
	typedef net::Session<MAX_BUF_LEN, tcp_preactor_t, MyConnection<Pack, ObjCollection, Dispatchers...> > base_t;
	
public:
	MyConnection(net::fd_t fd, const char *ip, unsigned short port)
		:base_t(fd, ip, port)
	{
//		_push_sender = shared_ptr<UdpPeer>( UdpSock::Create() );
		base_t::ss_container_t::instance().put(this);
	}

	~MyConnection()
	{
		base_t::ss_container_t::instance().pop(this);
	}

	void InputHandle(size_t len)
	{
#ifdef DEBUG
		cout<<"recv("<<len<<"):"<<base_t::_inbuf+8<<endl;
		fs<<"recv("<<len<<"):"<<base_t::_inbuf+8<<endl;
#endif
		pack_t pck;
		_userial(pck, base_t::_inbuf, len);
		if(pck.status() )
		{
			//ArgIteration<Dispachters...>::Handle(ObjCollection::Instance(), pck, _replies);
			RequestContext context = {base_t::_fd, base_t::_remoteip, base_t::_remote_port};
			typename pack_t::pack_list_t pcks = DispatcherHandler<Dispatchers...>::Handle(context, ObjCollection::Instance(), pck);
			for(auto &i : pcks)
				_replies.push_back(i);
			
		}		
		else
		{
#ifdef DEBUG
			cout<<"pack error"<<endl;
#endif
		}
	}

	void OutputHandle()
	{
#ifdef DEBUG
		cout<<"OutputHandle()"<<endl;
#endif
		vector<const char*> bufs;
		vector<size_t> bufsizes;
		for (auto &ip : _replies)
		{
			size_t bufsize=0;
			typename pack_t::serial_t serial;
			bufs.push_back( serial(ip, &bufsize) );
			bufsizes.push_back(bufsize);
			base_t::_outbuf_size += bufsize;
		}
		
		if( base_t::_outbuf != nullptr)
		{
			delete[] base_t::_outbuf;
			base_t::_outbuf=nullptr;
		}
		base_t::_outbuf = new char[base_t::_outbuf_size];
		for(size_t i=0; i<bufs.size(); i++)
		{
			if(i==0)
				memcpy(base_t::_outbuf, bufs[0], bufsizes[0]); 
			else
				memcpy(base_t::_outbuf+bufsizes[i-1], bufs[i], bufsizes[i]); 

			delete[] bufs[i];
		}

		_replies.clear();
	}
	

	int PostOutput(typename pack_t::pack_ptr_t pck)
	{
		cout<<"PostOutput()"<<endl;
		_replies.push_back(pck);
		base_t::_preactor->PostSend(base_t::_fd);
	}

private:
	typename pack_t::pack_list_t _replies;
	typename pack_t::unserial_t _userial = typename pack_t::unserial_t(MAX_BUF_LEN);
};


typedef MyConnection<Jpack,objects_t
	,member_login_dispth
	,member_add_dispth
	,tranmsg_dispth
> myconnection_t;

//typedef SessionContainer<session_t > ss_container;
typedef typename myconnection_t::ss_container_t conn_container;
