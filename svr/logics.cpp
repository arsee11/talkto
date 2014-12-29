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
	else
	{
		if ((*i)->pwd() != key)
		{
			_rsp->ReplyAdd("msg", "pwd not match");
			(*i)->Login(rev->ip, rev->port);
			return 2;
		}
	}

	return 0;
}

//TransMsgTo
int TransMsgTo::Execute(Receiver* rev, member_list_obj_t *obj, size_t from, size_t to, const string &msg)
{
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
	
	typename ss_container::session_ptr_t ss =ss_container::instance().get( 
			(*mto)->loginip()
			,(*mto)->login_port()
	);

	auto sender = bind(&session_t::PostOutput, ref(*ss), placeholders::_1, placeholders::_2);
	_rsp->PushAdd("msg", msg);
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
