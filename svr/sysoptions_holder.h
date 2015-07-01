//sysoptions_holder.h

#ifndef SYSOPTIONS_HOLDER_H	
#define SYSOPTIONS_HOLDER_H	

#ifndef STRINGEX_H
#include "stringex.h"
#endif

#include <string>
#include "odb/modelsys-odb.hxx"
#include "odb/dbholder.h"

using namespace std;


class SysoptionsHolder
{
public:
	static SysoptionsHolder& instance()
	{
		static SysoptionsHolder myself;
		return myself;
	}

	size_t timeout(){ return _timeout; }
	int    port(){ return _port; }
	
private:
	SysoptionsHolder()
	{
		Load();
	}

	SysoptionsHolder(SysoptionsHolder&);
	SysoptionsHolder& operator=(SysoptionsHolder&);


	void Load()
	{
		typedef odb::result<Sys> res_t;
		odb::transaction t(DbConnPool::instance().get()->begin());
		try{
			res_t res( DbConnPool::instance().get()->query<Sys>() );
			for(res_t::iterator i(res.begin()); i!=res.end(); ++i)
			{
				Sys& sys = (*i);
				if(sys.name()=="timeout")
					_timeout = arsee::str2t<size_t>(sys.value());
			}
		}
		catch(odb::exception& e){
			DbErrorHandle(e);
		}
	}

	size_t _timeout=1*60; //seconds
	size_t _onlines=0	;
	size_t _max_onlines=10	;
	int    _port=11111	;	
};

#endif/*SYSOPTIONS_HOLDER_H*/	
