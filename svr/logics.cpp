#include "logics.h"
#include <algorithm>

//AddMember

const string AddMember::name = "addmember";
const string AddMember::_p1 = "Id";
const string AddMember::_p2 = "Name";

int AddMember::Execute(obj_t *obj)
{
	return 0;
}

int AddMember::Execute(obj_t *obj, const string &id, const string& name)
{
	member_ptr_t mem = member_ptr_t(new member_obj_t(id, name));;
	obj->ref().push_back(mem);
	return 0;
}



//MemberLogin
const string MemberLogin::name = "login";
const string MemberLogin::_p1 = "Id";
const string MemberLogin::_p2 = "Key";

int MemberLogin::Execute(member_list_obj_t *obj, const string &id, const string& key)
{
	if (obj == nullptr)
		return 1;

	auto i = find_if(obj->ref().begin(), obj->ref().end(),
		[&id](const member_ptr_t &mem){ return mem->ref().Id() == id; }
	);
		
	if (i == obj->ref().end() )
	{
		return 1;
	}
	else
	{
		//if ((*i)->ref().Key() != key)
		if ( "abc" != key )
			return 2;
	}

	return 0;
}

