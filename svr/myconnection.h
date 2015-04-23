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

#ifndef RECEIVER_H 
#include "mvc/receiver.h"
#endif

#ifndef TCPSOCK_H 
#include "net/tcpsock.h"
#endif

#include <vector>

using namespace std;
using namespace arsee;

typedef net::Preactor<net::FdHolder, true, net::Epoll> tcp_preactor_t;

template<class Pack, class ObjCollection, class... Dispatchers>
class MyConnection :
	public net::Session<1024, tcp_preactor_t, MyConnection<Pack, ObjCollection, Dispatchers...> >
{
	typedef Pack pack_t;
	typedef net::Session<1024, tcp_preactor_t, MyConnection<Pack, ObjCollection, Dispatchers...> > base_t;
	
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
#endif
		pack_t pck;
		_userial(pck, base_t::_inbuf, len);
		if(pck.status() )
		{
			//ArgIteration<Dispachters...>::Handle(ObjCollection::Instance(), pck, _replies);
			Receiver rev = {base_t::_fd, base_t::_remoteip, base_t::_remote_port};
			ArgIteration<Dispatchers...>::Handle(rev, ObjCollection::Instance(), pck, _replies);
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
		//typename pack_t::serial_t serial;
		//size_t len=0;
		//const char *buf = serial(pck, &len);
		//AddrPair addr = {11112, base_t::_remoteip};
//		_push_sender->Write(buf, len, addr);
	}

private:
	typename pack_t::pack_list_t _replies;
	typename pack_t::unserial_t _userial = typename pack_t::unserial_t(1024);
	
//	shared_ptr<UdpPeer> _push_sender;
};


typedef MyConnection<Jpack,objects_t
	,member_login_dispth
	,member_add_dispth
	,tranmsg_dispth
> myconnection_t;

//typedef SessionContainer<session_t > ss_container;
typedef typename myconnection_t::ss_container_t conn_container;
