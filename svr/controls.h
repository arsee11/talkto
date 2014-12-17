#pragma once

#ifndef CONTROLS_H
#define CONTROLS_H

#ifndef MVC_H
#include "/mvc.h"

#ifndef MEMBER_H
#include "member.h"
#endif

#ifndef MSG_H
#include "msg.h"
#endif

#ifndef LOGICS_H
#include "logics.h"
#endif

using namespace arsee;

typedef typename MvcTrait<MemberLogin, member_list_obj_t>::dispth_t member_login_dispth;
typedef typename MvcTrait<AddMember, member_list_obj_t>::dispth_t member_add_dispth;


//typedef arsee::RControl<member_obj_t, AddMember, arsee::RRequest, udpchannel_t> member_ctrl_t;
//typedef member_list_ctrl_t::response_t member_list_rsp_t;
//typedef Dispatcher<member_ctrl_t> member_dispth;


//typedef arsee::Control<msg_obj_t, arsee::Request, arsee::Response> msg_ctrl_t;
//typedef msg_ctrl_t::request_t msg_rqt_t;
//typedef msg_ctrl_t::response_t msg_rsp_t;



#endif /*CONTROLS_H*/
