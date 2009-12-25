#include <stdio.h>
#include <string.h>
#include "object.h"

struct Object* lookup_method(struct Object *o, char *name)
{
	if (name == NULL || o == NULL)
		return NULL;

	struct Object *met;
	
	int i;
	for (i = 0; met != nil_object; i++) {
		met = (struct Object *) list_nth(o->methods, i);
		if (strcmp(name, met->name) == 0)
			return met;
	}

	return nil_object;
}

struct Object* new_object(void)
{

	struct Object *new = (struct Object *) zalloc(sizeof(struct Object));
	new->methods = clone_object(list_object);

	return new;
}

struct Object* clone_object(struct Object *o)
{
	if (o == NULL)
		return NULL;

	struct Object *new = (struct Object *) zalloc(sizeof(struct Object));
	new->super = o->super;
	new->type = o->type;
	new->name = strndup(o->name, 255); /* FIXME : test the value of strdup */

	struct Object *met;

	int i;
	for (i = 0; i < list_length(o->methods); i++) {
		met = (struct Object *) list_nth(o->methods, i);
		/* FIXME : Copy the method if it's a block,
		   	   to be able to free() later.
		 */

		list_append(new->methods, met);
	}


	return new;
}

void free_object(struct Object *o)
{
	if (o == NULL)
		return;

	free(o->name);

	/* FIXME : free : the methods list, the methods and finally the object */
	free(o);
}

void init_object_system(void)
{
	/* We need to create the list object before everything else because it is
	   needed for the method dictionnaries.
	 */
	create_list_object(); 

	/* init nil object */
	nil_object = new_object();
	nil_object->super = NULL;
	nil_object->type = T_NIL;
	nil_object->name = "nil";


	/* init base object */
	base_object = new_object();
	base_object->super = nil_object;
	base_object->type = T_OBJECT;
	base_object->name = "Object";
	
	/* init number object */
	number_object = new_object();
	number_object->super = base_object;
	number_object->type = T_NUMBER;
	number_object->name = "Number";
	number_object->value.i_value = 0;

}
