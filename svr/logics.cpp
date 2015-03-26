//logics.cpp

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
