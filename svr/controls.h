#pragma once

#ifndef CONTROLS_H
#define CONTROLS_H

#ifndef MVC_H
#include "mvc/mvc.h"
#endif


#ifndef MSG_LOGICS_H
#include "msg_logics.h"
#endif

#ifndef MEMBER_LOGICS_H
#include "member_logics.h"
#endif

#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

#ifndef OBJECTS_H 
#include "objects.h"
#endif


using namespace arsee;



typedef typename MvcTrait<Jpack, objects_t, MemberLogin>::dispth_t member_login_dispth;
typedef typename MvcTrait<Jpack, objects_t, AddMember>::dispth_t member_add_dispth;
typedef typename MvcTrait<Jpack, objects_t, TransMsgTo>::dispth_t tranmsg_dispth;



#endif /*CONTROLS_H*/
