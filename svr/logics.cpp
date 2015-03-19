#include "logics.h"
#include <algorithm>
#include <iostream>
#include "myconnection.h"


//AddMember
TransMsgTo::response_t* AddMember::Execute(Receiver* rev, member_list_obj_t *obj, size_t id, const string& name)
{
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("msg", "request OK");
	cout<<"AddMember::Execute"<<endl;
	member_ptr_t mem = member_ptr_t(new Member(id, name));;
	obj->ref().push_back(mem);
	
	//persistent handle
	
	return rsp;
}



//MemberLogin
TransMsgTo::response_t* MemberLogin::Execute(Receiver* rev, member_list_obj_t *obj, size_t id, const string& key)
{
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("msg", "request faild!");
	
	if (obj == nullptr)
		return rsp;

	auto i = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->id() == id; }
	);
		
	if (i == obj->ref().end() )
	{
		rsp->ParamAdd("msg", "member not exist");
		return rsp;
	}
		//if ((*i)->pwd() != key)
	if (string("123")!= key)
	{
		rsp->ParamAdd("msg", "pwd not match");
		return rsp;
	}

	(*i)->ip(rev->ip);
	(*i)->port(rev->port);
	rsp->ParamAdd("msg", "request OK");
	
	return rsp;
}

//TransMsgTo
TransMsgTo::response_t* TransMsgTo::Execute(Receiver* rev, member_list_obj_t *obj, size_t from, size_t to, const string &msg)
{
	cout<<"TransMsgTo::Execute(..from="<<from<<",to="<<to<<",msg="<<msg<<")"<<endl;
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("msg", "request faild!");
	
	if (obj == nullptr)
		return rsp;

	auto mfrom = find_if(obj->ref().begin(), obj->ref().end(),
		[from](const member_ptr_t &mem){ return mem->id() == from; }
	);
	
	auto mto = find_if(obj->ref().begin(), obj->ref().end(),
		[to](const member_ptr_t &mem){ return mem->id() == to; }
	);
		
	if (mto == obj->ref().end() )
		return rsp;
	

	cout<<"tranto:"<<(*mto)->ip()<<(*mto)->port()<<endl;
	PushResponse<Jpack> pusher("pusher", "msgview");
	pusher.ParamAdd("msg", msg);
	pusher.ParamAdd("from",from);
	pusher.Push<conn_container>((*mto)->ip(), (*mto)->port() );
	
	rsp->ParamAdd("msg", "request OK!");
	return rsp;
}

//MemberLogin
TransMsgTo::response_t* MemberInfo::Execute(Receiver* rev, member_list_obj_t *obj, size_t id)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->ParamAdd("msg", "request faild!");
		return rsp;
	}

	auto i = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->id() == id; }
	);
		
	if (i == obj->ref().end() )
	{
		rsp->ParamAdd("msg", "member not exist");
		return rsp;
	}

	rsp->ParamAdd("id", (*i)->id());
	rsp->ParamAdd("name", (*i)->name());
	rsp->ParamAdd("ip", (*i)->ip());
	rsp->ParamAdd("port", (*i)->port());

	return rsp;
}
