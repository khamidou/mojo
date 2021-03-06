#ifndef __MOJO_OBJECT__
#define __MOJO_OBJECT__

#include <string.h>
#include "queue.h"
#include "number.h"
#include "zalloc.h"

/*
  The types :
  Note that a block is actually a list that is gonna be interpreted in a different environment.
*/
enum o_types {
	T_NIL,
	T_BOOL,
	T_NUMBER,
    T_SYMBOL,
	T_LIST,
	T_STRING,
	T_OBJECT, 
	T_BUILTIN,
	T_BLOCK,
    T_MESSAGE,
    T_SEPARATOR, /* ugly - used by the interpretor to separate statements in the AST */

}; /* the base types */

/* this cast is to silence some warnings sent by the compiler */
#define BUILTIN_CAST (struct Object * (*)(struct Object *parent, struct Object *arg1,\
					  struct Object *arg2, struct Object *arg3, struct Object *arg4))

struct Object {
	struct Object *super;
	char *name;
	enum o_types type;

	union {
		signed int i_value;
		char c_value;
		struct Object * (*c_method)(struct Object *parent, struct Object *arg1, struct Object *arg2, 
					    struct Object *arg3, struct Object *arg4); 
		struct mojo_list *l_value;
        struct mojo_string *s_value;
		struct Object *m_block;
        struct Object *v_value; /* the value pointed to by a symbol */
	} value;

	struct Object *methods; /* A list object */
	struct Object *symtab;  /* The list of symbols defined at this point */
};

struct Object *lookup_method(struct Object *o, char *name);
struct Object* lookup_variable(struct Object *scope, char *name);
struct Object *new_object(void);
struct Object *clone_object(struct Object *o);
void display_object(struct Object *obj, int nindents);
void free_object(struct Object *o);

struct Method *add_method(struct Object *o, struct Object * (*c_method)(struct Object *parent, struct Object *arg1, struct Object *arg2),
			  char *name);
struct Method *clone_method(struct Method *m);
void free_method(struct Method *m);

void init_object_system(void);

struct Object *nil_object;
struct Object *base_object;
struct Object *builtin_object;
struct Object *number_object;
struct Object *list_object;


#endif
