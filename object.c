#include <stdio.h>
#include <string.h>
#include "object.h"

struct Method *lookup_method(struct Object *o, char *name)
{
	if (name == NULL || o == NULL)
		return NULL;

	struct Method *m;

	TAILQ_FOREACH(m, &o->methods_head, methods) {
		if (strcmp(name, m->name) == 0)
			return m;
	}

	return NULL;
}

struct Object *new_object(void)
{

	struct Object *new = (struct Object *) zalloc(sizeof(struct Object));

	TAILQ_INIT(&new->methods_head);

	return new;
}

struct Object *clone_object(struct Object *o)
{
	if (o == NULL)
		return NULL;

	struct Object *new = (struct Object *) zalloc(sizeof(struct Object));
	new->super = o->super;
	new->type = o->type;
	new->name = strndup(o->name, 255); /* FIXME : test the value of strdup */

	struct Method *m;
	struct Method *new_method;
	TAILQ_INIT(&new->methods_head);
	TAILQ_FOREACH(m, &o->methods_head, methods) {
		new_method = clone_method(m);
		TAILQ_INSERT_TAIL(&new->methods_head, new_method, methods);
	}

	return new;
}

void free_object(struct Object *o)
{
	if (o == NULL)
		return;

	free(o->name);

	struct Method *m;
	TAILQ_FOREACH(m, &o->methods_head, methods) {
		free_method(m);
	}
}

struct Method *add_method(struct Object *o, struct Object * (*c_method)(struct Object *parent, struct Object *arg1, struct Object *arg2),
			    char *name)
{
	if (o == NULL)
		return NULL;

	struct Method *new = (struct Method *) zalloc(sizeof(struct Method));
	new->name = strndup(name, 255);
	new->c_method = c_method;
	new->parent = o;

	TAILQ_INSERT_TAIL(&o->methods_head, new, methods);	

	return new;
}

struct Method *clone_method(struct Method *m)
{
	if (m == NULL)
		return NULL;

	struct Method *new = (struct Method *) zalloc(sizeof(struct Method));
	
	new->name = strndup(m->name, 255);
	new->c_method = m->c_method;
	new->parent = m->parent;

	return new;
}

void free_method(struct Method *m)
{
	if (m == NULL)
		return;
	TAILQ_REMOVE(&m->parent->methods_head, m, methods); 
	free(m->name);
	free(m);
}

struct Object* Plus_method(struct Object *parent, struct Object *arg1, struct Object *arg2)
{
	if (parent == NULL || arg1 == NULL)
		return NULL;

	if (arg1->type != T_NUMBER)
		return nil_object;

	struct Object *ret = clone_object(number_object);
	ret->value.i_value = parent->value.i_value + arg1->value.i_value;

	return ret;
}

void init_object_system(void)
{
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
	add_method(number_object, &Plus_method, "+");

	/* init list object */
	list_object = new_object();
	list_object->super = base_object;
	list_object->type = T_LIST;
	list_object->name = "List";

}
