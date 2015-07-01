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
	string name()const{ return _name;}	
	string value()const{ return _value;}	
	
	void name (string val){ _name  = val;}
	void value(string val){ _value = val;}
	
private:
	#pragma db id 
	string _name;
	string _value;
};

///////////////////////////////////////////////////////////////////yy
#pragma db object
class Session
{
	friend class odb::access;
	
public:
	Session(){}
	
	size_t 		id		()const{ return _id		;}	
	string 		remote_ip	()const{ return _remote_ip	;}
	unsigned short 	remote_port 	()const{ return _remote_port ;}
	size_t 		member_id 	()const{ return _member_id ;}
	size_t		last_active 	()const{ return _last_active ;}
	string 		login_time 	()const{ return _login_time ;}
	
	void remote_ip	(string 		val){ _remote_ip	= val;}
	void member_id	(size_t 		val){ _member_id	= val;}
	void id		(size_t 		val){ _id		= val;}
	void last_active(size_t			val){ _last_active	= val;}
	void login_time	(string 		val){ _login_time	= val;}
	void remote_port(unsigned short 	val){ _remote_port 	= val;}
	
	
private:
	#pragma db id auto
	size_t 			_id		;	
	size_t 			_member_id	;
	string 			_remote_ip	;
	size_t			_last_active	;

	#pragma db value_type("datetime")
	string 			_login_time	;

	unsigned short 	_remote_port;
};

#endif /*MODLESYS_H*/
