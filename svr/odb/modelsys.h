//modelsys.h
//absract: persistent objects about system level config.

#ifndef MODLESYS_H
#define MODLESYS_H

#include <odb/core.hxx>
#include <odb/lazy-ptr.hxx>
#include <string>

using namespace std;
using namespace odb;

#pragma db object
class Sys
{
	friend class odb::access;
	
public:
	size_t 			onlines			()const{ return _onlines		;}	
	size_t 			time_out		()const{ return _time_out		;}	
	size_t 			max_onlines		()const{ return _max_onlines	;}	
	int 			port			()const{ return _port			;}	
	
	void onlines		(size_t val){ _onlines		= val;}
	void time_out		(size_t val){ _time_out		= val;}
	void max_onlines	(size_t val){ _max_onlines	= val;}
	void port			(int	val){ _port			= val;}
	
private:
	size_t _onlines		;
	size_t _time_out	;
	size_t _max_onlines	;
	int    _port		;	
};

///////////////////////////////////////////////////////////////////yy
#pragma db object
class Session
{
	friend class odb::access;
	
public:
	Session(){}
	
	size_t 			id			()const{ return _id			;}	
	string 			remote_ip	()const{ return _remote_ip	;}
	unsigned short 	remote_port ()const{ return _remote_port ;}
	unsigned size_t member_id 	()const{ return _member_id ;}
	unsigned string last_time 	()const{ return _last_time ;}
	
	void remote_ip	(string 		val){ _remote_ip	= val;}
	void member_id	(size_t 		val){ _member_id	= val;}
	void last_time	(string 		val){ _last_time	= val;}
	void remote_port(unsigned short val){ _remote_port 	= val;}
	
	
private:
	#pragma db id auto
	size_t 			_id			;	
	size_t 			_member_id	;
	string 			_remote_ip	;
	string 			_last_time	;

	unsigned short 	_remote_port;
};

#endif /*MODLESYS_H*/