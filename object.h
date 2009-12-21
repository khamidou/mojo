#ifndef __MOJO_OBJECT__
#define __MOJO_OBJECT__

#include "queue.h"
#include "zalloc.h"


enum o_types {
	T_NIL,
	T_BOOL,
	T_NUMBER,
	T_LIST,
	T_STRING,
	T_OBJECT, 
	T_BLOCK,
}; /* the base types */

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
	struct Object * (*c_method)(struct Object *parent, struct Object *arg1, struct Object *arg2); 
	
	TAILQ_ENTRY(Method) methods;

};

struct Object {
	struct Object *super;
	char *name;
	enum o_types type;

	union {
		int i_value;
		char c_value;
	} value;

	TAILQ_HEAD(Methods_head, Method) methods_head;
};

struct Method *lookup_method(struct Object *o, char *name);
struct Object *new_object(void);
struct Object *clone_object(struct Object *o);
void free_object(struct Object *o);

struct Method *add_method(struct Object *o, struct Object * (*c_method)(struct Object *parent, struct Object *arg1, struct Object *arg2),
			  char *name);
struct Method *clone_method(struct Method *m);
void free_method(struct Method *m);

void init_object_system(void);

struct Object *nil_object;
struct Object *base_object;
struct Object *number_object;

#endif
