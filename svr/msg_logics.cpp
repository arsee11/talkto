#include <algorithm>
#include <iostream>
#include "myconnection.h"
#include "odb/models-odb.hxx"
#include "msg_logics.h"
#include "logics.h"
#include "codes.h"

#ifndef LOGIC_SESSION_H 
#include "logic_session.h"
#endif

////////////////////////////////////////////////////////////////////
//TransMsgTo
TransMsgTo::response_t* TransMsgTo::Execute(
	Receiver* rev, friends_obj_t *obj, size_t from, size_t to, const string &msg)
{
	cout<<"TransMsgTo::Execute(..from="<<from<<",to="<<to<<",msg="<<msg<<")"<<endl;
	response_t *rsp = new response_t("response");
	rsp->add_param("code", RspCode::Failed);
		
	if (obj == nullptr)
	{
		rsp->append_param();
		return rsp;
	}

	member_ptr_t mto;
	if( (mto=GetObjectT<Member>(to) ) == nullptr )
	{
		rsp->append_param();
		return rsp;
	}

	if (!IsFriendWith(obj->ref(), from, to) )
	{
		rsp->append_param();
		return rsp;
	}	
	
	if( !SessionLogic().IsLogin(mto) )
	{
		rsp->append_param();
		//not login, the save the offline msg.
		return rsp;
	}
	
	cout<<"tranto:"<<mto->session()->remote_ip()<<mto->session()->remote_port()<<endl;
	PushResponse<Jpack> pusher("pusher", "msgview");
	pusher.add_param("msg", msg);
	pusher.add_param("from",from);
	pusher.append_param();
	pusher.Push<conn_container>(mto->session()->remote_ip(), mto->session()->remote_port() );
	
	//save msg history?
	
	rsp->add_param("code", RspCode::OK);
	rsp->append_param();
	return rsp;
}

