#pragma once
#ifndef MSG_LOGICS_H
#define MSG_LOGICS_H

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

class TransMsgTo :
	public BaseLogicJpack<3, TransMsgTo, size_t, size_t, string>
{
public:
	response_t* Execute(Receiver *rev, friends_obj_t *obj, size_t from, size_t to, const string &msg);

	static const string name(){ return "tran_msg"; }
	static const string target(){ return member_list_obj_t::name(); }
	static const string p1(){ return "from"; }
	static const string p2(){ return "to"; }
	static const string p3(){ return "msg"; }
};


#endif/*MSG_LOGICS_H*/
