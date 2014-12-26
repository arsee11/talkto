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

using namespace arsee;

typedef Preactor<FdHolder, true, Epoll> tcp_preactor_t;

template<class Pack, class ObjCollection, class... Dispachters>
class MySession :
	//public net::Session
	public Session<1024, tcp_preactor_t>
{
	typedef Pack pack_t;
	
public:
	MySession(fd_t fd, const char *ip, unsigned short port)
		:Session(fd, ip, port)
	{}
	void InputHandle(size_t len)
	{
#ifdef DEBUG
		cout<<"recv("<<len<<"):"<<_inbuf<<endl;
#endif
		typename pack_t::unserial_t userial(1024);
		pack_t pck;
		userial(pck, _inbuf, len);
		if(pck.Status() )
		{
			//ArgIteration<Dispachters...>::Handle(ObjCollection::Instance(), pck, _replies);
			Receiver rev = {_fd, _remoteip, _remote_port};
			ArgIteration<Dispachters...>::Handle(rev, ObjCollection::Instance(), pck, _replies);
		}
		
		
	}

	void OutputHandle()
	{
		vector<const char*> bufs;
		vector<size_t> bufsizes;
		for (auto &ip : _replies)
		{
			size_t bufszie=0;
			typename pack_t::serial_t serial;
			bufs.push_bck( serial(ip, &bufsize) );
			bufsizes.push_back(bufsize);
			_outbuf_size += bufsize;
		}
		
		if( _outbuf != nullptr)
		{
			delete[] _outbuf;
			_outbuf=nullptr;
		}
		_outbuf = new char[_outbuf_size];
		for(size_t i=0; i<bufs.size(); i++)
		{
			memcpy(_outbuf+i*bufsizes[i], bufs[i], bufsizes[i]); 
			delete[] bufsizes[i];
		}
	}
	

private:
	typename pack_t::pack_list_t _replies;
};


typedef MySession<Jpack,objects_t, member_login_dispth> session_t;
typedef typename session_t::ss_container_t ss_container;
