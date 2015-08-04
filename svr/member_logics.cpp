#include "member_logics.h"
#include <algorithm>
#include <iostream>
#include "myconnection.h"
#include "odb/models-odb.hxx"
#include "codes.h"
#include "logics.h"

#ifndef LOGIC_SESSION_H
#include "logic_session.h"
#endif

bool IsMemberExist(size_t id)
{
	return GetObjectT<Member>(id) != nullptr;
}


////////////////////////////////////////////////////////////////////
//AddMember
AddMember::response_t* AddMember::Execute(member_list_obj_t *obj, const string& name, const string& pwd)
{
	response_t *rsp = new response_t("response");
	rsp->add_param("code", RspCode::OK);
	cout<<"AddMember::Execute"<<endl;
	member_ptr_t mem = member_ptr_t(new Member(name));
	mem->pwd(pwd);
	//member_list maxsize=? 存放最近活跃的？
	obj->ref().push_back(mem);
	
	//persistent handle
	size_t id = 0;
	if( !Persist( DbConnPool::instance().get(), (*mem), id ) )
		rsp->add_param("code", RspCode::Failed);
	
	rsp->add_param("id", id);
	rsp->append_param();
	return rsp;
}


////////////////////////////////////////////////////////////////////
//MemberLogin
MemberLogin::response_t* MemberLogin::Execute(member_list_obj_t *obj, size_t id, const string& key)
{
#ifdef DEBUG
	cout<<"member_login called!"<<endl;
#endif
	cout<<"key="<<key<<endl;

	response_t *rsp = new response_t("response");
	rsp->add_param("code", RspCode::Failed);
	
	string rawkey;
	if (obj == nullptr)
	{
		rsp->append_param();
		return rsp;
	} 

	member_ptr_t m;
	if ((m = GetObjectT<Member>(id)) == nullptr)
	{
		rsp->append_param();
		return rsp;
	}
		
	if (m->pwd() != key)
	{
		rsp->append_param();
		return rsp;
	}

	SessionLogic().Login(m, get_request_context()->ip, get_request_context()->port);

	try{
		odb::transaction t(DbConnPool::instance().get()->begin());
		DbConnPool::instance().get()->update<Member>(m);
		t.commit();
	}
	catch (odb::exception& e){
		DbErrorHandle(e);
		rsp->append_param();
		return rsp;
	}

	rsp->add_param("code", RspCode::OK);
	rsp->append_param();
	return rsp;
}


////////////////////////////////////////////////////////////////////
//MemberLogin
MemberInfo::response_t* MemberInfo::Execute(member_list_obj_t *obj, size_t id)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->add_param("code", RspCode::Failed);
		return rsp;
	}

	member_ptr_t m;
	if ((m = GetObjectT<Member>(id)) == nullptr)
	{
		rsp->add_param("code", RspCode::Failed);
		return rsp;
	}
	
	rsp->add_param("code", RspCode::OK);
	rsp->add_param("id", m->id());
	rsp->add_param("name", m->name());
	rsp->add_param("ip", m->session()->remote_ip());
	rsp->add_param("port", m->session()->remote_port());
	rsp->append_param();
	
	return rsp;
}

////////////////////////////////////////////////////////////////////
//Make Friend
MakeFriendAction::response_t* MakeFriendAction::Execute(
	friends_obj_t *obj, size_t me, size_t who)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->add_param("code", RspCode::Failed);
		return rsp;
	}

	if( IsFriendWith(obj->ref(), me, who) )
	{
		rsp->add_param("code", RspCode::HadBeenYourFriend);
		return rsp;
	}
	
	member_ptr_t mwho;
	if( (mwho = GetObjectT<Member>(who) ) == nullptr )
	{
		rsp->add_param("code", RspCode::MemberNotExists);
		return rsp;
	}
	
	//notify to the target member
	PushResponse<Jpack> pusher("fiends_notify_view");
	//PushResponse<Jpack> pusher("pusher", MakeFriendAction::name()); //action -> view 
	pusher.add_param("code", RspCode::MakeFriendRequest);
	pusher.add_param("who",me);
	pusher.append_param();
	pusher.Push<conn_container>(mwho->session()->remote_ip(), mwho->session()->remote_port() );
	
	rsp->add_param("code", OK);
	rsp->append_param();
	return rsp;
}


////////////////////////////////////////////////////////////////////
//Make Friend
AcceptFriendAction::response_t* AcceptFriendAction::Execute(
	friends_obj_t *obj, size_t me, size_t who, bool op, string msg)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->add_param("code", RspCode::Failed);
		rsp->append_param();
		return rsp;
	}

	member_ptr_t mwho;
	if( (mwho = GetObjectT<Member>(who) ) == nullptr )
	{
		rsp->add_param("code", RspCode::Failed);
		rsp->append_param();
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
	pusher.add_param("code", op?RspCode::MakeFriendArgee:RspCode::MakeFriendRefuse);
	pusher.add_param("who",me);
	pusher.add_param("msg", msg);
	pusher.append_param();
	pusher.Push<conn_container>(mwho->session()->remote_ip(), mwho->session()->remote_port() );
	
	rsp->add_param("code", OK);
	rsp->append_param();
	return rsp;
}

////////////////////////////////////////////////////////////////////
//MemberLogin
FriendInfo::response_t* FriendInfo::Execute(friends_obj_t *obj, size_t who)
{
	response_t *rsp = new response_t("response");
	
	if (obj == nullptr)
	{
		rsp->add_param("code", RspCode::Failed);
		rsp->append_param();
		return rsp;
	}

	member_ptr_t mwho;
	if( (mwho = GetObjectT<Member>(who) ) == nullptr )
	{
		rsp->add_param("code", RspCode::Failed);
		rsp->append_param();
		return rsp;
	}
	
	if(obj->ref().size() > 0 )
	{
		PushResponse<Jpack> p("firends_view");
		for(auto i : obj->ref())
		{
			p.add_param("who"	,i->y()->id());
			p.add_param("weight"	,i->w());
			p.append_param();			
		}
		p.Push<conn_container>( mwho->session()->remote_ip(), mwho->session()->remote_port() );
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
			PushResponse<Jpack> p("firends_view");
			for(result_t::iterator i(r.begin()); i != r.end(); ++i)
			{
				member_ptr_t x(i->x().load());
				member_ptr_t y(i->y().load());
				p.add_param("who"	,y->id());
				p.add_param("weight",i->w());
				p.append_param();
				obj->ref().push_back( relation_ptr_t( new RelationNetwork(x->id(), y->id(), i->w() ) ) );
			}
			p.Push<conn_container>( mwho->session()->remote_ip(), mwho->session()->remote_port() );
		}
		catch (odb::exception& e){
			DbErrorHandle(e);
			return rsp;
		}
	}
	
	rsp->add_param("code", RspCode::OK);
	rsp->append_param();
	return rsp;
}
