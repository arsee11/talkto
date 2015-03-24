#include "member_logics.h"
#include <algorithm>
#include <iostream>
//#include "myconnection.h"
#include "odb/models-odb.hxx"
#include "odb/dbholder.h"
#include "codes.h"



template<class Obj, class Id>
shared_ptr<Obj> GetObjectT(Id id)
{
	member_list_obj_t *mems = objects_t::Instance().GetObj<member_list_obj_t>();
	if( mems == nullptr)
		return false;
		
	auto i = find_if(mems->ref().begin(), mems->ref().end(),
			[id](member_ptr_t m){ return id == m->id(); }
	);
	
	if (i == mems->ref().end())
	{
		try{
			odb::transaction t( DbConnPool::instance().get()->begin() );
			shared_ptr<Member> m( DbConnPool::instance().get()->load<Member>( id ) );
			return m;
		}
		catch(odb::exception& e){
			return nullptr;
		}
	}
	
	return (*i);
}

bool IsMemberExist(size_t id)
{
	return GetObjectT<Member>(id) != nullptr;
}

bool IsFriendWith(const friends_t& friends, size_t me, size_t who)
{
	auto i = find_if(friends.begin(), friends.end(),
		[&who](const relation_ptr_t &f){ return f->y()->id() == who; }
	);
		
	if (i != friends.end() )
	{
		return true;
	}
	
	typedef odb::query<RelationNetwork> query_t;
	typedef odb::result<RelationNetwork> result_t;
	try{
		//odb::session ss;
		odb::transaction t( DbConnPool::instance().get()->begin() );
		result_t r( DbConnPool::instance().get()->query<RelationNetwork>(
				  query_t::x==me && query_t::y==who)
		);
		
		if( r.size() > 0 )
		{
			return true;
		}
	}
	catch(odb::exception& e)
	{
		DbErrorHandle(e);
		return false;
	}
	
	return false;	
}

////////////////////////////////////////////////////////////////////
//AddMember
AddMember::response_t* AddMember::Execute(Receiver* rev, member_list_obj_t *obj, const string& name)
{
	response_t *rsp = new response_t("response");
	rsp->ParamAdd("code", RspCode::OK);
	cout<<"AddMember::Execute"<<endl;
	member_ptr_t mem = member_ptr_t(new Member(name));
	//member_list maxsize=? 存放最近活跃的？
	obj->ref().push_back(mem);
	
	//persistent handle
	if( !Persist( DbConnPool::instance().get(), (*mem) ) )
		rsp->ParamAdd("code", RspCode::Failed);
		
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

	auto i = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->id() == id; }
	);
		
	if (i == obj->ref().end() )
	{
		typedef odb::query<Member> query_t;
		typedef odb::result<Member> result_t;
		try{
			//odb::session ss;
			DbConnPool::db_ptr_t db = DbConnPool::instance().get();
			odb::transaction t(db->begin());
			result_t r(db->query<Member>(query_t::id == id));
			if (r.size() == 0)
			{
				return rsp;
			}
			else
				rawkey = r.begin()->pwd();
		}
		catch (odb::exception& e){
			return rsp;
			DbErrorHandle(e);
		}
		
	}
	else
		rawkey = (*i)->pwd();
		
		//if ((*i)->pwd() != key)
	if (rawkey != key)
	{
		return rsp;
	}

	(*i)->ip(rev->ip);
	(*i)->port(rev->port);
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
	//pusher.Push<conn_container>(mwho->ip(), mwho->port() );
	
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
	//pusher.Push<conn_container>(mwho->ip(), mwho->port() );
	
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
			//p.Push<conn_container>( mwho->ip(), mwho->port() );
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
			result_t r( db->query<RelationNetwork>(query_t::x==who) );
			for(result_t::iterator i(r.begin()); i != r.end(); ++i)
			{
				PushResponse<Jpack> p("firends_view");
				p.ParamAdd("who"	,i->y()->id());
				p.ParamAdd("weight"	,i->w());
				//p.Push<conn_container>( mwho->ip(), mwho->port() );
				obj->ref().push_back( relation_ptr_t( new RelationNetwork(i->x()->id(), i->y()->id(), i->w() ) ) );
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
