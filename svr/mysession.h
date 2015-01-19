//mysession.h


#include "controls.h"
#include "mvc/utility.h"

#ifndef SESSIOIN_H
#include "net/session.h"
#endif

#include "net/preactor.h"
#include "net/fd.h"

#ifndef SELECTOR_H
#include "net/selector.h"
#endif

#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

#ifndef CONTROLS_H
#include "controls.h"
#endif

#ifndef MEMBER_H
#include "member.h"
#endif

#ifndef RECEIVER_H 
#include "mvc/receiver.h"
#endif

#ifndef TCP_SOCK_H 
#include "net/udpsock.h"
#endif

#include <vector>

using namespace std;
using namespace arsee;

typedef Preactor<FdHolder, true, Epoll> tcp_preactor_t;

template<class Pack, class ObjCollection, class... Dispatchers>
class MySession :
	//public net::Session
	public Session<1024, tcp_preactor_t, MySession<Pack, ObjCollection, Dispatchers...> >
{
	typedef Pack pack_t;
	typedef Session<1024, tcp_preactor_t, MySession<Pack, ObjCollection, Dispatchers...> > base_t;
	
public:
	MySession(fd_t fd, const char *ip, unsigned short port)
		:base_t(fd, ip, port)
	{
		_push_sender = shared_ptr<UdpPeer>( UdpSock::Create() );
		base_t::ss_container_t::instance().put(this);
	}

	~MySession()
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
			memcpy(base_t::_outbuf+i*bufsizes[i], bufs[i], bufsizes[i]); 
			delete[] bufs[i];
		}

		_replies.clear();
	}
	

	int PostOutput(const pack_t &pck)
	{
		//_replies.push_back(pck);
		//base_t::_preactor->PostSend(base_t::_fd);
		typename pack_t::serial_t serial;
		size_t len=0;
		const char *buf = serial(pck, &len);
		AddrPair addr = {11112, base_t::_remoteip};
		_push_sender->Write(buf, len, addr);
	}

private:
	typename pack_t::pack_list_t _replies;
	typename pack_t::unserial_t _userial = typename pack_t::unserial_t(1024);
	
	shared_ptr<UdpPeer> _push_sender;
};


typedef MySession<Jpack,objects_t
	,member_login_dispth
	,member_add_dispth
	,tranmsg_dispth
> mysession_t;

//typedef SessionContainer<session_t > ss_container;
typedef typename mysession_t::ss_container_t ss_container;
