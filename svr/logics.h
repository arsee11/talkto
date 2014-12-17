#pragma once
#ifndef LOGICS_H
#define LOGICS_H

#ifndef MVC_REQUEST_H
#include "mvc/mvcrequest.h"
#endif

#ifndef MEMBER_H
#include "member.h"
#endif


#ifndef PARAMS_DISPATCH_H
#include "mvc/params_dispatch.h"
#endif

class AddMember :
	public arsee::BaseLogicTmpl<2, AddMember, string, string>
{
public:
	AddMember(IResponse *rsp)
		:base_t(rsp)
	{
	}
	
	int Execute(obj_t *obj, const string &id, const string& name);

	const static string name;
};

class MemberLogin:
	public arsee::BaseLogicTmpl<2, MemberLogin, string, string>
{
public:
	MemberLogin(IResponse *rsp)
		:base_t(rsp)
	{
	}
	
	int Execute(member_list_obj_t *obj, const string &id, const string& key);

	const static string name;
};

class TransMsgTo :
	public arsee::BaseLogicTmpl<3, AddMember, string, string>
{
public:
	TransMsgTo(IResponse *rsp)
		:base_t(rsp)
	{
	}
	
	int Execute(obj_t *obj, const string &id, const string& name);

	const static string name;
};

#endif/*LOGICS_H*/