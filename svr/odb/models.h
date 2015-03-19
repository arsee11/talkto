//models.h
#ifndef MODELS_H
#define MODELS_H

#include <odb/core.hxx>
#include <odb/laze-ptr.hxx>
#include <string>
#include <list>
#include <vector>

using namespace std;
using namespace odb;

#pragma db model version(1,1)

#pragma db object
class Member
{
	friend class odb::access;
	
public:
	Member()
		:_name("member")
	{}


	Member(size_t id, const string &name)
		:_id(id)
		, _name(name)
	{}

	string 			ip	()const{ return _ip	; }
	unsigned short 	port()const{ return _port; }
	string 			name()const{ return _name; }
	string 			pwd	()const{ return _pwd ; }
	size_t 			id	()const{ return _id  ; }
	
	void  ip	(const string& 	val){ _ip	= val; }
	void  port 	(unsigned short val){ _port	= val; }
	void  name	(const string& 	val){ _name = val; }
	void  pwd	(const string& 	val){ _pwd	= val; }
	void  id	(size_t 		val){ _id	= val; }
	
	
private:
	string 			_name	;
	string 			_pwd	;
	string 			_ip		;
	unsigned short  _port	;

	vector<lazy_shared_ptr<Member> > _friends;
	
	#pragma db id auto
	size_t _id = 0;
};


///////////////////////////////////////////////////////////////////yy
#pragma db object
class RelationNetWork
{
	friend class odb::access;
public:
	RelationNetWork()
	{
	}

private:
	size_t 	_x; //member x	
	size_t 	_y; //member y
	int 	_w; //weitght
	
	#pragma db id auto
	size_t 	_id;

};


///////////////////////////////////////////////////////////////////yy
#pragma db object
class Group 
{
	friend class odb::access;
public:
	Group()
	{
	}

private:
	#pragma db value_not_null
	list<shared_ptr<Member>> _x;

	#pragma db id auto
	size_t _id	;	
	string _name;
};


///////////////////////////////////////////////////////////////////yy
#pragma db object
class Msg
{
	friend class odb::access;
public:
	Msg(){}

private:
	lazy_shared_ptr<Member> 		_from	;
	list<lazy_shared_ptr<Member> > 	_to		;
	string 							_content;
	
	#pragma db id auto
	size_t 							_id		;
};

///////////////////////////////////////////////////////////////////yy
#pragma db object
class Session
{
	friend class odb::access;
public:
	Session(){}
	
private:
	#pragma db id auto
	size_t 			_id			;	
	size_t 			_member_id	;
	string 			_remote_ip	;
	unsigned short 	_remote_port;
};

#endif /*MODELS_H*/
