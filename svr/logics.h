#pragma once
#ifndef LOGICS_H
#define LOGICS_H

#ifndef MEMBER_H
#include "member.h"
#endif

#ifndef LOGIC_TMPL_H
#include "mvc/logictmpl.h"
#endif

#ifndef RECEIVER_H 
#include "mvc/receiver.h"
#endif

using namespace arsee;

class AddMember :
	public BaseLogicJpack<2, AddMember, size_t, string>
{
public:
	response_t* Execute(Receiver *rev, member_list_obj_t *obj, size_t id, const string& name);

	static const string name(){ return "registry"; }
	static const string target(){ return member_list_obj_t::name(); }
	static const string p1(){ return "id"; }
	static const string p2(){ return "name"; }
};

class MemberLogin:
	public BaseLogicJpack<2, MemberLogin, size_t, string>
{
public:
	response_t* Execute(Receiver *rev, member_list_obj_t *obj, size_t id, const string& key);

	static const string name(){ return "login"; }
	static const string target(){ return member_list_obj_t::name(); }
	static const string p1(){ return "id"; }
	static const string p2(){ return "key"; }
};

class TransMsgTo :
	public BaseLogicJpack<3, TransMsgTo, size_t, size_t, string>
{
public:
	response_t* Execute(Receiver *rev, member_list_obj_t *obj, size_t from, size_t to, const string &msg);

	static const string name(){ return "tran_msg"; }
	static const string target(){ return member_list_obj_t::name(); }
	static const string p1(){ return "from"; }
	static const string p2(){ return "to"; }
	static const string p3(){ return "msg"; }
};

class MemberInfo:
	public BaseLogicJpack<1, MemberLogin, size_t>
{
public:
	response_t* Execute(Receiver *rev, member_list_obj_t *obj, size_t id);

	static const string name(){ return "member_info"; }
	static const string target(){ return member_list_obj_t::name(); }
	static const string p1(){ return "id"; }
};

#endif/*LOGICS_H*/
