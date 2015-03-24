//models.h
#ifndef MODELS_H
#define MODELS_H

#include <odb/core.hxx>
#include <odb/lazy-ptr.hxx>
#include <string>
#include <list>
#include <vector>
#include <utility>

using namespace std;
using namespace odb;

#pragma db model version(1,1)

#pragma db object
class Member
{
	friend class odb::access;
	
public:
	Member(){}


	Member(const string &name)
		:_name(name)
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

	//vector<lazy_shared_ptr<Member> > _friends;
	
	#pragma db id auto
	size_t _id = 0;
};


///////////////////////////////////////////////////////////////////yy
#pragma db object
class RelationNetwork
{
	friend class odb::access;
public:
	RelationNetwork()
	{
	}
	
	RelationNetwork(size_t	x 
	                ,size_t y 
	                ,short 	w 	
	)
		:_x(new Member())
		,_y(new Member())
		,_w(w)
	{
		_x->id(x);
		_y->id(y);
	}
		
	lazy_shared_ptr<Member>	x (){ return _x	;}
	lazy_shared_ptr<Member>	y (){ return _y	;}
	short 		   			w (){ return _w	;}
	size_t 					id(){ return _id;}
	
	void x (lazy_shared_ptr<Member>	val){ _x  = val;}
	void y (lazy_shared_ptr<Member>	val){ _y  = val;}
	void w (short 		   			val){ _w  = val;}
	
private:
	lazy_shared_ptr<Member>	_x; //member x	
	lazy_shared_ptr<Member>	_y; //member y
	short 		   			_w; //weitght
	
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

	Group(const string& name)
		:_name(name)
	{
	}
	
	list<shared_ptr<Member>> members(){ return std::move(_members)	;}
	size_t 					 id		(){ return _id					;}
	string 					 name	(){ return _name				;}
	
	void members(list<shared_ptr<Member>>	val){ _members	= val;}
	void name	(string 					val){ _name		= val;}
	
private:
	#pragma db value_not_null
	list<shared_ptr<Member>> _members;

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
	
	Msg( lazy_shared_ptr<Member> 		 from	
	     ,list<lazy_shared_ptr<Member> > to		
	     ,const string&					 content 
	)
		:_from(from)
		,_to(to)
		,_content(content)
	{}
	
	lazy_shared_ptr<Member> 		from	(){ return _from	;}
	list<lazy_shared_ptr<Member> > 	to		(){ return _to		;}
	string 							content (){ return _content ;}
	size_t 							id		(){ return _id		;}
		
	void from	(lazy_shared_ptr<Member> 		val){ _from	   = val; }
	void to		(list<lazy_shared_ptr<Member> > val){ _to	   = val; } 
	void content(string 						val){ _content = val; }	
	
private:
	lazy_shared_ptr<Member> 		_from	;
	list<lazy_shared_ptr<Member> > 	_to		;
	string 							_content;
	
	#pragma db id auto
	size_t 							_id		;
};


#endif /*MODELS_H*/
