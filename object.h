#ifndef __MOJO_OBJECT__
#define __MOJO_OBJECT__

#include "queue.h"

enum { 
       T_BOOL,
       T_INT,
       T_LIST,
       T_STRING,
       T_OBJECT, 
       T_BLOCK,
} o_types; /* the base types */

/*
  A method is actually a function pointer.
  What about methods defined in mojo ?
  for instance, 3 square ?
  where square is defined as : : square -> self * self;
  On a un handler par défaut qui appelle les sous-methodes et retourne le résultat.
*/

struct Method {
	char *name;
	struct Object *parent;
	struct Object * (*c_method)(struct Object *arg1, struct Object *arg2); 
	
	TAILQ_ENTRY(Method) methods;

};

struct Object {
	struct Object *super;
	char *name;
	o_types type;
	TAILQ_HEAD(Methods_head, Methods) methods_head;
};

struct Method *lookup_method(struct Object *o);

#endif
