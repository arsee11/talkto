#include "logics.h"
#include <algorithm>
#include <iostream>
#include "myconnection.h"
#include "odb/models-odb.hxx"
#include "odb/dbholder.h"


////////////////////////////////////////////////////////////////////
//TransMsgTo
TransMsgTo::response_t* TransMsgTo::Execute(
	Receiver* rev, friends_obj_t *obj, size_t from, size_t to, const string &msg)
{
	cout<<"TransMsgTo::Execute(..from="<<from<<",to="<<to<<",msg="<<msg<<")"<<endl;
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("code", REQUEST_FAILED);
	rsp->ParamAdd("msg", "request faild!");
	
	if (obj == nullptr)
		return rsp;

	auto mto = find_if(obj->ref().begin(), obj->ref().end(),
		[to](const relation_ptr_t &r){ return r->y() == to; }
	);
		
	if (mto == obj->ref().end() )
	{
		typedef odb::query<RelationNetwork> query_t;
		typedef odb::result<RelationNetwork> result_t;
		try{
			//odb::session ss;
			odb::transaction t( DbConnPool::instance().get()->begin() );
			result r( DbConnPool::instance().get()->query<RelationNetwork>(
					  query_t::x==from && query::y==to)
			);
			
			if( r.size() == 0 )
			{
				rsp->ParamAdd("code", NOT_YOUR_FRIEND);
				rsp->ParamAdd("msg", "not your friend!");
				return rsp;
			}
		}
		catch(odb::exception& e)
		{
			DbHandleError(e);
			return rsp;
		}
			
			//Add to friends_obj_t;
	}
	

	cout<<"tranto:"<<(*mto)->ip()<<(*mto)->port()<<endl;
	PushResponse<Jpack> pusher("pusher", "msgview");
	pusher.ParamAdd("msg", msg);
	pusher.ParamAdd("from",from);
	pusher.Push<conn_container>((*mto)->ip(), (*mto)->port() );
	
	rsp->ParamAdd("msg", "request OK!");
	return rsp;
}

