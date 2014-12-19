#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <list>
#include <memory>
#include <xutility>

#ifndef MVC_OBJECT_H
#include "mvc/mvcobject.h"
#endif

#ifndef MODELS_H
#include "odb/models.h"
#endif

using namespace std;

typedef arsee::Object<Member> member_obj_t;
typedef shared_ptr<member_obj_t> member_ptr_t;
typedef struct member_list :public list< member_ptr_t >
{
	//const string Name(){ return name(); }
	const string name(){ return "memberlist"; }
	
} member_list_t;

typedef arsee::Object<member_list_t> member_list_obj_t;
typedef arsee::ObjectsCollection<member_list_obj_t> objects_t;


#endif /*MEMBER_H*/