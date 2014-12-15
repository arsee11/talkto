#ifndef MSG_H
#define MSG_H

#include <string>
#include <list>
#include <memory>
using namespace std;

#ifndef MVC_OBJECT_H
#include "mvcobject.h"
#endif


typedef arsee::Object<Msg> msg_obj_t;
typedef shared_ptr<msg_obj_t> msg_ptr_t;
typedef list< msg_ptr_t > msg_list_t;


#endif/*MSG_H*/
