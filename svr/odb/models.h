//models.h
#ifndef MODELS_H
#define MODELS_H

#include <string>

using namespace std;

#pragma db object
class Member
{
public:
	Member()
		:_name("member")
	{}


	Member(size_t id, const string &name)
		:_id(id)
		, _name(name)
	{}


	void Login(const string& ip, unsigned short port){ _ip = ip; _port = port; }
	string loginip(){ return _ip;}
	unsigned short login_port(){ return _port;}
	string name(){ return _name; }
	string pwd(){ return _pwd; }
	size_t id(){ return _id; }
	
private:
	string _name;
	string _pwd;
	string _ip;
	unsigned short _port;

	#pragma db id auto
	size_t _id = 0;
};


///////////////////////////////////////////////////////////////////yy
#pragma db object
class Relationship
{
public:
	Relationship()
	{
	}

private:
	Member _x;
	Member _y;

	int _r;

	#pragma db id auto
	size_t _id;

};


///////////////////////////////////////////////////////////////////yy
#pragma db object
class Group 
{
public:
	Group()
	{
	}

private:
	list<Member> _x;

	#pragma db id auto
	size_t _id;

};


///////////////////////////////////////////////////////////////////yy
#pragma db object
class Msg
{
public:
	Msg(){}

private:
	Member _from;
	list<Member> _to;

	string _context;

	#pragma db id auto
	size_t _id;
};

#endif /*MODELS_H*/
