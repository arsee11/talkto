#include <algorithm>
#include <iostream>
#include "myconnection.h"
#include "odb/models-odb.hxx"
#include "odb/dbholder.h"
#include "msg_logics.h"
#include "logics.h"
#include "codes.h"

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

	auto mto = find_if(obj->ref().begin(), obj->ref().end(),
		[to](const relation_ptr_t &r){ return r->y()->id() == to; }
	);
		
	if (!IsFriendWith(obj->ref(), from, to) )
	{
		rsp->append_param();
		return rsp;
	}	

	member_ptr_t mem((*mto)->y().load());
	cout<<"tranto:"<<mem->ip()<<mem->port()<<endl;
	PushResponse<Jpack> pusher("pusher", "msgview");
	pusher.add_param("msg", msg);
	pusher.add_param("from",from);
	pusher.append_param();
	pusher.Push<conn_container>(mem->ip(), mem->port() );
	
	//save msg history?
	
	rsp->add_param("code", RspCode::OK);
	rsp->append_param();
	return rsp;
}

