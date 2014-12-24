#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <list>
#include <memory>
#include <utility>


#ifndef MODELS_H
#include "odb/models.h"
#endif

#ifndef MVC_OBJECT_H
#include "mvc/mvcobject.h"
#endif

#include "mvc/mvcresponse.h"

#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

using namespace std;

typedef arsee::RResponse<arsee::Jpack> rsp_t;
typedef std::shared_ptr<Member>  member_ptr_t;
typedef struct member_list :public list< member_ptr_t >
{
	//const string Name(){ return name(); }
	static const string name(){ return "memberlist"; }
	
} member_list_t;

typedef arsee::Object<member_list, rsp_t>  member_list_obj_t;
typedef arsee::ObjectsCollection<member_list_obj_t> objects_t;

#endif /*MEMBER_H*/
