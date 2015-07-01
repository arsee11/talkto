//logic_session.cpp

#ifndef LOGIC_SESSION_H
#define LOGIC_SESSION_H

#ifndef OBJECTS_H
#include "objects.h"
#endif

#ifndef SYSOPTIONS_HOLDER_H
#include "sysoptions_holder.h"
#endif

#ifndef SYSTIME_HOLDER_H
#include "systime_holder.h"
#endif

class SessionLogic
{
public:
	bool IsLogin(member_ptr_t& mem)
	{
		if( mem->session() == nullptr )
			return false;
				
		if( IsTimeout(mem->session().get()) )
			return false;
		
		return true;	
	}

	bool IsTimeout(const Session* s)
	{
		size_t time_out = SysoptionsHolder::instance().timeout();
		size_t now = SystimeHolder::Tick();
		
		if(now - s->last_active() > time_out)
			return true;

		return false;
	}

	bool Login(member_ptr_t& mem, const string& ip, unsigned short port)
	{
		Member::session_ptr s( new Session );
		s->remote_ip(ip);
		s->member_id(mem->id());
		s->remote_port(port);
		s->login_time(SystimeHolder::Now());
		s->last_active(SystimeHolder::Tick());
		odb::transaction t(DbConnPool::instance().get()->begin());
		t.tracer(stderr_tracer);
		//s->id(DbConnPool::instance().get()->persist(s));
		DbConnPool::instance().get()->persist(s);
		t.commit();

		//_sessions.push_back(s);
			
		mem->session(s);
		return true;
	}

	bool Logout(member_ptr_t& mem)
	{
		if( mem->session() == nullptr)
			return true;

		odb::transaction t(DbConnPool::instance().get()->begin());
		t.tracer(stderr_tracer);
		DbConnPool::instance().get()->erase<Session>(mem->session()->id());
		t.commit();

		//_sessions.erase(mem->session());
		mem->session(nullptr);
		return true;
	}

private:
	//session_list_t& _sessions;
};

#endif /*LOGIC_SESSION_H*/
