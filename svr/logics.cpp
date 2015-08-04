//logics.cpp

#include "objects.h"
#include "odb/dbholder.h"
#include "odb/models-odb.hxx"

bool IsFriendWith(const friends_t& friends, size_t me, size_t who)
{
	auto i = find_if(friends.begin(), friends.end(),
		[&who,&me](const relation_ptr_t &f){
			return f->x()->id()==me && f->y()->id() == who 
			       ||f->x()->id()==who&&f->y()->id()==me;
		}
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
		t.tracer(stderr_tracer);
		result_t r( DbConnPool::instance().get()->query<RelationNetwork>(
				  query_t::x==me && query_t::y==who
				  ||query_t::x==who&&query_t::y==me
				  )
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
