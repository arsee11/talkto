//member_logic.h

#ifndef MEMBER_LOGICS_H
#define MEMBER_LOGICS_H

#ifndef OBJECTS_H
#include "objects.h"
#endif

#ifndef LOGIC_TMPL_H
#include "mvc/logictmpl.h"
#endif

#ifndef RECEIVER_H 
#include "mvc/receiver.h"
#endif

using namespace arsee;

//pack protocol
//request********************
//soruce:member_view
//tartget:member_list
//action :registry
//params:who = {Member::id}
//response*******************
//target:member_view
//action:registry
//params:code=(OK|FAILED)
//*******************response
class AddMember :
	public BaseLogicJpack<2, AddMember, size_t, string, string>
{
public:
	response_t* Execute(Receiver *rev, member_list_obj_t *obj, const string& name, const string& pwd);

	static const string name(){ return "registry"; }
	static const string target(){ return member_list_obj_t::name(); }
	static const string p1(){ return "name"; }
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


//pack protocol
//request********************
//soruce:member_view
//tartget:member_list
//action :get_member_info
//params:who = {Member::id}
//response*******************
//target:member_view
//action:get_member_info
//params:{member}
//*******************response
class MemberInfo:
	public BaseLogicJpack<1, MemberLogin, size_t>
{
public:
	response_t* Execute(Receiver *rev, member_list_obj_t *obj, size_t id);

	static const string name(){ return "get_member_info"; }
	static const string target(){ return member_list_obj_t::name(); }
	static const string p1(){ return "id"; }
};


//pack protocol
//request********************
//soruce:fiends_view
//tartget:friends_list
//action :get_friends_list
//params:who = {Member::id}
//response*******************
//target:fiends_view
//action:get_friends_list
//params:{friends_list}
//*******************response
class FriendInfo:
	public BaseLogicJpack<1, FriendInfo, size_t>
{
public:
	response_t* Execute(Receiver *rev, friends_obj_t *obj, size_t who);
	
	static const string name(){ return "get_friend_list"; }
	static const string target(){ return friends_obj_t::name(); }
	static const string p1(){ return "who"; }
};


//pack protocol
//request********************
//soruce:fiends_notify_view
//tartget:friends_list
//action :make_friend_with
//params:name1=me, name2=who
//********************request
//response*******************
//target:quest
//action :make_friend_with
//params:code=[OK,FAILED]
//*******************response
//push*******************
//target:fiends_notify_view
//params:code=MAKE_FRIEND_REQUEST, who={Memeber::id}
//*******************push
class MakeFriendAction:
	public BaseLogicJpack<2, MakeFriendAction, size_t, size_t>
{
public:
	response_t* Execute(Receiver *rev, friends_obj_t *obj, size_t me, size_t who);
	
	static const string name(){ return "make_friend_with"; }
	static const string target(){ return friends_obj_t::name(); }
	static const string p1(){ return "me"; }
	static const string p2(){ return "who"; }
};


//pack protocol
//request********************
//source: fiends_notify_view
//tartget:friends_list
//action :accept_friend_with
//params:name1=me, name2=who
//********************request
//response*******************
//target:quest
//action :accept_friend_with
//params:code=(OK|FAILED)
//*******************response
//push*******************
//target:fiends_notify_view
//params:code=(REFUSE|AGREE), msg=(str), who={Memeber::id}
//*******************push
class AcceptFriendAction:
	public BaseLogicJpack<4, AcceptFriendAction, size_t, size_t, bool, string>
{
public:
	response_t* Execute(Receiver *rev, friends_obj_t *obj, size_t me, size_t who, bool op, string msg);
	
	static const string name(){ return "accept_friend_with"; }
	static const string target(){ return friends_obj_t::name(); }
	static const string p1(){ return "me"; }
	static const string p2(){ return "who"; }
};

#endif/*MEMBER_LOGICS_H*/
