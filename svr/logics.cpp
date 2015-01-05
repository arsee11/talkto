#include "logics.h"
#include <algorithm>
#include <iostream>
#include "mysession.h"


//AddMember
TransMsgTo::response_t* AddMember::Execute(Receiver* rev, member_list_obj_t *obj, size_t id, const string& name)
{
	cout<<"AddMember::Execute"<<endl;
	member_ptr_t mem = member_ptr_t(new Member(id, name));;
	obj->ref().push_back(mem);
	
	//persistent handle
	
	return 0;
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

	(*i)->Login(rev->ip, rev->port);
	rsp->ParamAdd("msg", "request OK");
	
	return rsp;
}

//TransMsgTo
TransMsgTo::response_t* TransMsgTo::Execute(Receiver* rev, member_list_obj_t *obj, size_t from, size_t to, const string &msg)
{
	cout<<"AddMember::Execute(..from="<<from<<",to="<<to<<",msg="<<msg<<")"<<endl;
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
	
	typename ss_container::session_ptr_t pss =ss_container::instance().get( 
			(*mto)->loginip()
			,(*mto)->login_port()
	);

	if(pss == nullptr)
		return rsp;

	//session_t ss = static_cast<session_t>(*pss);
	typedef typename ss_container::session_t session_t;
	cout<<"tranto:"<<pss->remote_ip()<<","<<pss->remote_port()<<endl;
	auto sender = bind(&session_t::PostOutput, ref(*pss), placeholders::_1);
	puser_t pusher("pusher", "msgview");
	pusher.ParamAdd("msgview", "msg", msg);
	pusher.Push(sender);
	
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
		rsp->ReplyAdd("msg", "member not exist");
		return rsp;
	}

	rsp->ReplyAdd("name", (*i)->name());
	rsp->ReplyAdd("loginip", (*i)->loginip());
	rsp->ReplyAdd("login_port", (*i)->login_port());

	return rsp;
}
