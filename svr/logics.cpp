#include "logics.h"
#include <algorithm>
#include <iostream>
#include "mysession.h"


//AddMember
int AddMember::Execute(Receiver* rev, member_list_obj_t *obj, size_t id, const string& name)
{
	cout<<"AddMember::Execute"<<endl;
	member_ptr_t mem = member_ptr_t(new Member(id, name));;
	obj->ref().push_back(mem);
	
	//persistent handle
	
	return 0;
}



//MemberLogin
int MemberLogin::Execute(Receiver* rev, member_list_obj_t *obj, size_t id, const string& key)
{
	if (obj == nullptr)
		return 1;

	auto i = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->id() == id; }
	);
		
	if (i == obj->ref().end() )
	{
		_rsp->ReplyAdd("msg", "member not exist");
		return 1;
	}
		//if ((*i)->pwd() != key)
	if (string("123")!= key)
	{
		_rsp->ReplyAdd("msg", "pwd not match");
		return 2;
	}

	(*i)->Login(rev->ip, rev->port);
	return 0;
}

//TransMsgTo
int TransMsgTo::Execute(Receiver* rev, member_list_obj_t *obj, size_t from, size_t to, const string &msg)
{
	cout<<"AddMember::Execute(..from="<<from<<",to="<<to<<",msg="<<msg<<endl;
	if (obj == nullptr)
		return 1;

	auto mfrom = find_if(obj->ref().begin(), obj->ref().end(),
		[from](const member_ptr_t &mem){ return mem->id() == from; }
	);
	
	auto mto = find_if(obj->ref().begin(), obj->ref().end(),
		[to](const member_ptr_t &mem){ return mem->id() == to; }
	);
		
	if (mto == obj->ref().end() )
		return 1;
	
	typename ss_container::session_ptr_t pss =ss_container::instance().get( 
			(*mto)->loginip()
			,(*mto)->login_port()
	);

	if(pss == nullptr)
		return 1;

	//session_t ss = static_cast<session_t>(*pss);
	typedef typename ss_container::session_t session_t;
	cout<<"tranto:"<<pss->remote_ip()<<","<<pss->remote_port()<<endl;
	auto sender = bind(&session_t::PostOutput, ref(*pss), placeholders::_1);
	_rsp->PushAdd("msgview", "msg", msg);
	_rsp->Push(sender);
	//mto->Update(p, sender);
	

	return 0;
}

//MemberLogin
int MemberInfo::Execute(Receiver* rev, member_list_obj_t *obj, size_t id)
{
	if (obj == nullptr)
		return 1;

	auto i = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->id() == id; }
	);
		
	if (i == obj->ref().end() )
	{
		_rsp->ReplyAdd("msg", "member not exist");
		return 1;
	}

	_rsp->ReplyAdd("name", (*i)->name());
	_rsp->ReplyAdd("loginip", (*i)->loginip());
	_rsp->ReplyAdd("login_port", (*i)->login_port());

	return 0;
}
