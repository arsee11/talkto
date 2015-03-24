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

private:
	size_t _onlines;
	size_t _max_onlines;
	int    _port;
	
};

///////////////////////////////////////////////////////////////////yy
#pragma db object
class Session
{
	friend class odb::access;
public:
	Session(){}
	
	size_t 			id			(){ return _id			;}	
	size_t 			member_id	(){ return _member_id	;}
	string 			remote_ip	(){ return _remote_ip	;}
	unsigned short 	remote_port (){ return _remote_port ;}
	
	void member_id	(size_t 		val){ _member_id	= val;}
	void remote_ip	(string 		val){ _remote_ip	= val;}
	void remote_port(unsigned short val){ _remote_port 	= val;}
	
	
private:
	#pragma db id auto
	size_t 			_id			;	
	size_t 			_member_id	;
	string 			_remote_ip	;
	unsigned short 	_remote_port;
};

#endif /*MODLESYS_H*/