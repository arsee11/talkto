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
	public arsee::BaseLogicTmpl<2, AddMember, rsp_t, size_t, string>
{
public:
	AddMember(rsp_t* rsp)
		:base_t(rsp)
	{
	}
	
	int Execute(Receiver *rev, member_list_obj_t *obj, size_t id, const string& name);

	static const string name(){ return "registry"; }
	static const string target(){ return member_list_obj_t::name(); }
};

class MemberLogin:
	public arsee::BaseLogicTmpl<2, MemberLogin, rsp_t, size_t, string>
{
public:
	MemberLogin(rsp_t* rsp)
		:base_t(rsp)
	{
	}
	
	int Execute(Receiver *rev, member_list_obj_t *obj, size_t id, const string& key);

	static const string name(){ return "login"; }
	static const string target(){ return member_list_obj_t::name(); }
};

class TransMsgTo :
	public arsee::BaseLogicTmpl<3, TransMsgTo, rsp_t, size_t, size_t, string>
{
public:
	TransMsgTo(rsp_t* rsp)
		:base_t(rsp)
	{
	}
	
	int Execute(Receiver *rev, member_list_obj_t *obj, size_t from, size_t to, const string &msg);

	static const string name(){ return "tran_msg"; }
	static const string target(){ return member_list_obj_t::name(); }
};

#endif/*LOGICS_H*/
