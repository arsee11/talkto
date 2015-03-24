//dbholder.h

#ifndef DBHOLDER_H
#define DBHOLDER_H

#include <odb/core.hxx>
#include <odb/mysql/database.hxx>


template<class Object>
inline bool Persist(database* db, Object& obj)throw( odb::exception)
{
	try{
		odb::session s;
		odb::transaction t( db->begin() );
		db->persist(obj);
		t.commit();
	}
	catch(odb::exception& e){
		//throw e;
		return false;
	}
	
	return true;
}

inline void DbErrorHandle(odb::exception& e)
{
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
			_dbs = auto_ptr<odb::database>(new odb::mysql::database("root", "114225", "localhost"));
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

#endif /*DBHOLDER_H*/