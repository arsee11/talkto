#pragma once

#ifndef CONTROLS_H
#define CONTROLS_H

#ifndef MVC_H
#include "mvc/mvc.h"
#endif


#ifndef MSG_LOGICS_H
#include "msg_logics.h"
#endif

#ifndef LOGICS_H
#include "member_logics.h"
#endif

#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

#ifndef RECEIVER_H 
#include "mvc/receiver.h"
#endif

using namespace arsee;



typedef typename MvcTrait<Jpack,Receiver, MemberLogin>::dispth_t member_login_dispth;
typedef typename MvcTrait<Jpack,Receiver, AddMember>::dispth_t member_add_dispth;
typedef typename MvcTrait<Jpack,Receiver, TransMsgTo>::dispth_t tranmsg_dispth;


//typedef arsee::RControl<member_obj_t, AddMember, arsee::RRequest, udpchannel_t> member_ctrl_t;
//typedef member_list_ctrl_t::response_t member_list_rsp_t;
//typedef Dispatcher<member_ctrl_t> member_dispth;


//typedef arsee::Control<msg_obj_t, arsee::Request, arsee::Response> msg_ctrl_t;
//typedef msg_ctrl_t::request_t msg_rqt_t;
//typedef msg_ctrl_t::response_t msg_rsp_t;



#endif /*CONTROLS_H*/
