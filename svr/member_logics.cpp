#include "member_logics.h"
#include <algorithm>
#include <iostream>
#include "myconnection.h"
#include "odb/models-odb.hxx"
#include "odb/dbholder.h"
#include "codes.h"
#include "logics.h"

bool IsMemberExist(size_t id)
{
	return GetObjectT<Member>(id) != nullptr;
}


////////////////////////////////////////////////////////////////////
//AddMember
AddMember::response_t* AddMember::Execute(Receiver* rev, member_list_obj_t *obj, const string& name, const string& pwd)
{
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("code", RspCode::OK);
	cout<<"AddMember::Execute"<<endl;
	member_ptr_t mem = member_ptr_t(new Member(name));
	mem->pwd(pwd);
	//member_list maxsize=? 存放最近活跃的？
	obj->ref().push_back(mem);
	
	//persistent handle
	size_t id = 0;
	if( !Persist( DbConnPool::instance().get(), (*mem), id ) )
		rsp->ParamAdd("code", RspCode::Failed);
	
	rsp->ParamAdd("id", id);
	return rsp;
}


////////////////////////////////////////////////////////////////////
//MemberLogin
MemberLogin::response_t* MemberLogin::Execute(Receiver* rev, member_list_obj_t *obj, size_t id, const string& key)
{
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("code", RspCode::Failed);
	
	string rawkey;
	if (obj == nullptr)
		return rsp;

	member_ptr_t m;
	if ((m = GetObjectT<Member>(id)) == nullptr)
	{
		return rsp;
	}
		
	if (m->pwd() != key)
	{
		return rsp;
	}

	m->ip(rev->ip);
	m->port(rev->port);
	try{
		odb::transaction t(DbConnPool::instance().get()->begin());
		DbConnPool::instance().get()->update<Member>(m);
		t.commit();
	}
	catch (odb::exception& e){
		DbErrorHandle(e);
		return rsp;
	}

	rsp->ParamAdd("code", RspCode::OK);
	
	return rsp;
}


////////////////////////////////////////////////////////////////////
//MemberLogin
MemberInfo::response_t* MemberInfo::Execute(Receiver* rev, member_list_obj_t *obj, size_t id)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->ParamAdd("code", RspCode::Failed);
		return rsp;
	}

	member_ptr_t m;
	if ((m = GetObjectT<Member>(id)) == nullptr)
	{
		rsp->ParamAdd("code", RspCode::Failed);
		return rsp;
	}
	
	rsp->ParamAdd("code", RspCode::OK);
	rsp->ParamAdd("id", m->id());
	rsp->ParamAdd("name", m->name());
	rsp->ParamAdd("ip", m->ip());
	rsp->ParamAdd("port", m->port());

	return rsp;
}

////////////////////////////////////////////////////////////////////
//Make Friend
MakeFriendAction::response_t* MakeFriendAction::Execute(
	Receiver* rev, friends_obj_t *obj, size_t me, size_t who)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->ParamAdd("code", RspCode::Failed);
		return rsp;
	}

	if( IsFriendWith(obj->ref(), me, who) )
	{
		rsp->ParamAdd("code", RspCode::HadBeenYourFriend);
		return rsp;
	}
	
	member_ptr_t mwho;
	if( (mwho = GetObjectT<Member>(who) ) == nullptr )
	{
		rsp->ParamAdd("code", RspCode::MemberNotExists);
		return rsp;
	}
	
	//notify to the target member
	PushResponse<Jpack> pusher("fiends_notify_view");
	//PushResponse<Jpack> pusher("pusher", MakeFriendAction::name()); //action -> view 
	pusher.ParamAdd("code", RspCode::MakeFriendRequest);
	pusher.ParamAdd("who",me);
	pusher.Push<conn_container>(mwho->ip(), mwho->port() );
	
	rsp->ParamAdd("code", OK);
	return rsp;
}


////////////////////////////////////////////////////////////////////
//Make Friend
AcceptFriendAction::response_t* AcceptFriendAction::Execute(
	Receiver* rev, friends_obj_t *obj, size_t me, size_t who, bool op, string msg)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->ParamAdd("code", RspCode::Failed);
		return rsp;
	}

	member_ptr_t mwho;
	if( (mwho = GetObjectT<Member>(who) ) == nullptr )
	{
		rsp->ParamAdd("code", RspCode::Failed);
		return rsp;
	}
	
	if(op)
	{
		///make frined and persist
		try{
			odb::transaction t( DbConnPool::instance().get()->begin() );
			RelationNetwork *rn = new RelationNetwork(me, who, 0);
			DbConnPool::instance().get()->persist(*rn);
			obj->ref().push_back(relation_ptr_t(rn));
			t.commit();
		}
		catch(odb::exception& e){
			DbErrorHandle(e);
			return rsp;
		}
	}
	
	//notify to the target member
	PushResponse<Jpack> pusher("fiends_notify_view");
	//PushResponse<Jpack> pusher("pusher", MakeFriendAction::name()); //action -> view 
	pusher.ParamAdd("code", op?RspCode::MakeFriendArgee:RspCode::MakeFriendRefuse);
	pusher.ParamAdd("who",me);
	pusher.ParamAdd("msg", msg);
	pusher.Push<conn_container>(mwho->ip(), mwho->port() );
	
	rsp->ParamAdd("code", OK);
	return rsp;
}

////////////////////////////////////////////////////////////////////
//MemberLogin
FriendInfo::response_t* FriendInfo::Execute(Receiver* rev, friends_obj_t *obj, size_t who)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->ParamAdd("code", RspCode::Failed);
		return rsp;
	}

	member_ptr_t mwho;
	if( (mwho = GetObjectT<Member>(who) ) == nullptr )
	{
		rsp->ParamAdd("code", RspCode::Failed);
		return rsp;
	}
	
	if(obj->ref().size() > 0 )
	{
		for(auto i : obj->ref())
		{
			//pack don't surppot param array yet
			//rsp->ParamArrayAdd("who", (*i)->y());
			//rsp->ParamArrayAdd("weight"(*i)->w());
			PushResponse<Jpack> p("firends_view");
			p.ParamAdd("who"	,i->y()->id());
			p.ParamAdd("weight"	,i->w());
			p.Push<conn_container>( mwho->ip(), mwho->port() );
		}
	}
	//load form persistent.
	else
	{
		typedef odb::query<RelationNetwork> query_t;
		typedef odb::result<RelationNetwork> result_t;
		try{
			DbConnPool::db_ptr_t db = DbConnPool::instance().get();
			odb::transaction t( db->begin() );
			t.tracer(stderr_tracer);
			result_t r( db->query<RelationNetwork>(query_t::x==who) );
			for(result_t::iterator i(r.begin()); i != r.end(); ++i)
			{
				PushResponse<Jpack> p("firends_view");
				member_ptr_t x(i->x().load());
				member_ptr_t y(i->y().load());
				p.ParamAdd("who"	,y->id());
				p.ParamAdd("weight"	,i->w());
				p.Push<conn_container>( mwho->ip(), mwho->port() );
				obj->ref().push_back( relation_ptr_t( new RelationNetwork(x->id(), y->id(), i->w() ) ) );
			}
		}
		catch (odb::exception& e){
			DbErrorHandle(e);
			return rsp;
		}
	}
	
	rsp->ParamAdd("code", RspCode::OK);
	return rsp;
}
