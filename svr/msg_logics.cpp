#include <algorithm>
#include <iostream>
#include "myconnection.h"
#include "odb/models-odb.hxx"
#include "odb/dbholder.h"
#include "msg_logics.h"
#include "logics.h"

////////////////////////////////////////////////////////////////////
//TransMsgTo
TransMsgTo::response_t* TransMsgTo::Execute(
	Receiver* rev, friends_obj_t *obj, size_t from, size_t to, const string &msg)
{
	cout<<"TransMsgTo::Execute(..from="<<from<<",to="<<to<<",msg="<<msg<<")"<<endl;
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("code", RspCode::Failed);
		
	if (obj == nullptr)
		return rsp;

	auto mto = find_if(obj->ref().begin(), obj->ref().end(),
		[to](const relation_ptr_t &r){ return r->y() == to; }
	);
		
	if (!IsFriendWith(obj->ref(), from, to) )
		return rsp;	

	cout<<"tranto:"<<(*mto)->ip()<<(*mto)->port()<<endl;
	PushResponse<Jpack> pusher("pusher", "msgview");
	pusher.ParamAdd("msg", msg);
	pusher.ParamAdd("from",from);
	pusher.Push<conn_container>((*mto)->ip(), (*mto)->port() );
	
	//save msg history?
	
	rsp->ParamAdd("code", RspCode::OK);
	return rsp;
}

