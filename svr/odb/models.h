//models.h
#ifndef MODELS_H
#define MODELS_H

#pragma db object
class Member
{
public:
	Member()
		:_id("")
		, _name("member")
	{}


	Member(const string &id, const string &name)
		:_id(id)
		, _name(name)
	{}

	Member(const char *id, const char *name)
		:_id(id)
		, _name(name)
	{}

	string Name(){ return _name; }
	string Pwd(){ return _pwd; }
	size_t Id(){ return _id; }
	
private:
	string _name;
	string _pwd;

	#pragma db id auto
	size_t _id;
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
