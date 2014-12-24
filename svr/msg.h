#ifndef MSG_H
#define MSG_H

#include <string>
#include <list>
#include <memory>
using namespace std;

#ifndef MVC_OBJECT_H
#include "mvc/mvcobject.h"
#endif

#ifndef MODELS_H
#include "odb/models.h"
#endif



typedef std::shared_ptr<Msg> msg_ptr_t;
typedef list< msg_ptr_t > msg_list_t;

#endif/*MSG_H*/
