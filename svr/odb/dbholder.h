//dbholder.h

#ifndef DBHOLDER_H
#define DBHOLDER_H

#include <odb/core.hxx>
#include <odb/mysql/database.hxx>
#include <memory>
#include <iostream>


using namespace std;


inline void DbErrorHandle(odb::exception& e)
{
	cout << e.what() << endl;
}

class DbConnPool
{
	enum{
		MaxConn = 1
	};

public:
	typedef odb::database* db_ptr_t;

public:
	static DbConnPool& instance()
	{
		static DbConnPool _myself;
		return _myself;
	}
	
	odb::database* get(){ return _dbs.get(); }

private:
	DbConnPool()
	{
		try{
			_dbs = auto_ptr<odb::database>(new odb::mysql::database("root", "114225", "talkto", "127.0.0.1"));
		}
		catch (odb::exception& e)
		{
			DbErrorHandle(e);
			//_dbs = nullptr;
		}
	}
	DbConnPool(const DbConnPool&);
	DbConnPool& operator=(const DbConnPool&);
	
	auto_ptr<odb::database> _dbs;
	
};

template<class Object, class Id>
inline bool Persist(database* db, Object& obj, Id& id)throw( odb::exception)
{
	try{
		odb::session s;
		odb::transaction t( db->begin() );
		id = db->persist(obj);
		t.commit();
	}
	catch(odb::exception& e){
		DbErrorHandle(e);
		return false;
	}
	
	return true;
}

template<class Obj, class Id>
shared_ptr<Obj> GetObjectT(Id id)
{
	member_list_obj_t *mems = objects_t::Instance().GetObj<member_list_obj_t>();
	if (mems == nullptr)
		return false;

	//get from memory
	auto i = find_if(mems->ref().begin(), mems->ref().end(),
		[id](member_ptr_t m){ return id == m->id(); }
	);

	//get from persistent
	if (i == mems->ref().end())
	{
		try{
			odb::transaction t(DbConnPool::instance().get()->begin());
			t.tracer(stderr_tracer);
			shared_ptr<Member> m(DbConnPool::instance().get()->load<Member>(id));
			return m;
		}
		catch (odb::exception& e){
			return nullptr;
		}
	}

	return (*i);
}



#endif /*DBHOLDER_H*/
