#include "logics.h"
#include <algorithm>
#include <iostream>
//AddMember

const string AddMember::name = "addmember";
template<>
const string AddMember::base_t::_p1 = "id";
template<>
const string AddMember::base_t::_p2 = "name";

int AddMember::Execute(obj_t *obj)
{
	return 0;
}

int AddMember::Execute(obj_t *obj, const string &id, const string& name)
{
	cout<<"AddMember::Execute"<<endl;
	member_ptr_t mem = member_ptr_t(new member_obj_t(id, name));;
	obj->ref().push_back(mem);
	
	//persistent handle
	
	return 0;
}



//MemberLogin
const string MemberLogin::name = "login";
template<>
const string MemberLogin::base_t::_p1 = "id";
template<>
const string MemberLogin::base_t::_p2 = "key";

int MemberLogin::Execute(member_list_obj_t *obj, const string &id, const string& key)
{
	if (obj == nullptr)
		return 1;

	auto i = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->ref().Id() == id; }
	);
		
	if (i == obj->ref().end() )
	{
		_rsp->AddParam("msg", "member not exist");
		return 1;
	}
	else
	{
		if ((*i)->ref().Pwd() != key)
		{
			_rsp->AddParam("msg", "pwd not match");
			return 2;
		}
	}

	return 0;
}

//TransMsgTo
const string TransMsgTo::name = "transMsg";
template<>
const string TransMsgTo::base_t::_p1 = "from";

template<>
const string TransMsgTo::base_t::_p2 = "to"

template<>
const string TransMsgTo::base_t::_p3 = "msg";

int TransMsgTo::Execute(member_list_obj_t *obj, const size_t &from, const size_t &to, const string &msg)
{
	if (obj == nullptr)
		return 1;

	auto &mfrom = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->ref().Id() == from; }
	);
	
	auto &mto = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->ref().Id() == to; }
	);
		
	if (mto == obj->ref().end() )
		return 1;
	
	param_t p;
	p["msg"] = msg;
	
	svr_t::session_list_t *ss =svr_t::GetSession(to);
	mto->Update(p, ss);
	

	return 0;
}

